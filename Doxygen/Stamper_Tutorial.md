\tableofcontents
# Stamper Tutorial {#Stamper_Tutorial}

## Introduction {#Intro_Stamper}
The purpose of this tutorial is to provide explanation on how to use the class defined in xmsstamper to trace a given point through a velocity vector field grid. The example provided in this tutorial refers to a test case that is in the xmsstamper/stamper/XmStamper.cpp source file.

## Example - Simple Stamper {#Example_Simple_Stamper}
This is the "hello world" example for using the stamper library.

This example shows how to insert a man-made structure into a natural topography or bathymetry set.

![Simple XmStamper](xmsstamper_tutorial.png)

The basic steps to define a linear stamped feature include:

1. Define the stamping coverage and centerline (or focal point) of the stamped feature.
2. Assign attributes to the centerline including the elevation profile along the centerline and the cross sections along the centerline.
3. Stamp the feature. This converts the centerline and its attributes to another coverage containing all the extents and details of the feature and a scatter set defining the elevation for the feature.

\snippet xmsstamper/stamper/XmStamper.cpp snip_test_Example_XmStamper
