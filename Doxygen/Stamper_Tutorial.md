\tableofcontents
# Stamper Tutorial {#Stamper_Tutorial}

## Introduction {#Intro_Stamper}
The purpose of this tutorial is to provide explanation on how to use the class defined in xmsstamper to trace a given point through a velocity vector field grid. The example provided in this tutorial refers to a test case that is in the xmsstamper/stamper/detail/XmStampInterpCrossSection.cpp source file.

## Example - Simple Stamper with Cross Sections {#Example_Simple_Stamper}
This is the "hello world" example for using the stamper library.

This example shows how to insert a man-made structure into a natural topography or bathymetry set.

The basic steps to define a linear stamped feature include:

1. Define the stamping coverage and centerline (or focal point) of the stamped feature.
2. Assign attributes to the centerline including the elevation profile along the centerline and the cross sections along the centerline.
3. Stamp the feature. This converts the centerline and its attributes to another coverage containing all the extents and details of the feature and a scatter set defining the elevation for the feature.

![Simple XmStamper Cross Section Tutorial](xmsstamper_testCrossSectionTutorial.PNG)

In this example we specify the black centerline and the 2 green cross sections just in from each end, with their shoulder points.  Then we stamp the feature to calculate the green interpolated shoulder lines and green cross sections at the other centerline points with 2 outside and 2 between the specified cross sections.

\snippet xmsstamper/stamper/detail/XmStampInterpCrossSection.cpp snip_test_Example_XmStamper_testCrossSectionTutorial
