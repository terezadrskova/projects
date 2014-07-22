#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <highgui.h>
#include <iostream>

using namespace cv;
using namespace std;


void MainWindow::drawSubdivPoint( Mat& img, Point2f fp, Scalar color )
{
    // circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
    circle(img, fp, 3, color, 1, 8, 0);
}

void MainWindow::drawSubdiv(Mat& img, Subdiv2D& subdiv, Scalar delaunayColor)
{
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point> pt(3);

    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
        line(img, pt[0], pt[1], delaunayColor, 1, 8, 0);
        line(img, pt[1], pt[2], delaunayColor, 1, 8, 0);
        line(img, pt[2], pt[0], delaunayColor, 1, 8, 0);
    }
}

void MainWindow::locatePoint( Mat& img, Subdiv2D& subdiv, Point2f fp, Scalar activeColor )
{
    int e0 = 0, vertex = 0;

    subdiv.locate(fp, e0, vertex);

    if( e0 > 0 )
    {
        int e = e0;
        do
        {
            Point2f org, dst;
            if( subdiv.edgeOrg(e, &org) > 0 && subdiv.edgeDst(e, &dst) > 0 )
                line( img, org, dst, activeColor, 3, 8, 0 );

            e = subdiv.getEdge(e, Subdiv2D::NEXT_AROUND_LEFT);
        }
        while( e != e0 );
    }

    drawSubdivPoint(img, fp, activeColor);
}


void MainWindow::paintVoronoi(Mat& img, Subdiv2D& subdiv)
{
    vector<vector<Point2f> > facets;
    vector<Point2f> centers;
    subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

    vector<Point> ifacet;
    vector<vector<Point> > ifacets(1);

    for( size_t i = 0; i < facets.size(); i++ )
    {
        ifacet.resize(facets[i].size());
        for( size_t j = 0; j < facets[i].size(); j++ )
            ifacet[j] = facets[i][j];

        // making background transparent
        //Scalar color(255,255,255);
        Scalar color;
        color[0] = finalVoronoiColors[i][2]*255*finalVoronoiIntensities[i][2];
        color[1] = finalVoronoiColors[i][1]*255*finalVoronoiIntensities[i][1];
        color[2] = finalVoronoiColors[i][0]*255*finalVoronoiIntensities[i][0];
        //qDebug() << "R: " << finalVoronoiColors[i][2] << "G: " << finalVoronoiColors[i][1] << "B: " << finalVoronoiColors[i][0];
        fillConvexPoly(img, ifacet, color, 8, 0);

        ifacets[0] = ifacet;
        polylines(img, ifacets, true, Scalar(), 1, 8, 0);
        circle(img, centers[i], 3, Scalar(), 1, 8, 0);
    }
}

/* ----------------------------------------------------------------
 *                VORONOI DIAGRAM
 *-----------------------------------------------------------------*/
void MainWindow::createVoronoiDiagram()
{
   extractFile(LIGHTDIRECTIONSPATH, true);
   extractFile(LIGHTINTENSITIESPATH, false);

   calculateDirections();
   calculateIntensities();

   Scalar active_facet_color(0, 0, 255), delaunay_color(255,255,255);

   Mat img = imread("/homes/td613/Documents/individual project/images/grace.ppm", 1);

   img = Scalar::all(0);

   for( int i = 0; i < NUMBEROFLIGHTSOURCES; i++ )
   {
       int phi = sphericalCoordinatesPX[i][0];
       int theta = sphericalCoordinatesPX[i][1];

       Point2f fp(phi,theta);
       locatePoint(img, subdiv, fp, active_facet_color);
       subdiv.insert(fp);

       img = Scalar::all(0);
       drawSubdiv(img, subdiv, delaunay_color);

   }
   calculateFinalIntensities(subdiv);

   img = Scalar::all(0);
   paintVoronoi(img, subdiv);

   const char* path = voronoiPathname.toStdString().c_str();
   imwrite(path, img);
}


