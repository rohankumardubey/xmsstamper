"""Test XmStamper_py.cpp"""
import unittest
import numpy as np
import os
import xmsstamper_py
import xmsinterp_py


class TestXmStamper(unittest.TestCase):
    """XmStamper tests"""
    def setUp(self):
        script_file = os.path.abspath(os.path.realpath(__file__))
        script_path = os.path.dirname(script_file)
        self.base_file_path = os.path.join(
            os.path.abspath(
                os.path.join(script_path, '..', '..', '..', 'test_files')
            )
        )
        self.assertTrue(os.path.isdir(self.base_file_path),
                        "Input file directory does not exist")
        self.output_file_path = os.path.join(
            os.path.abspath(
                os.path.join(script_path, '..', '..', '..', 'output_files')
            )
        )
        if not os.path.isdir(self.output_file_path):
            os.mkdir(self.output_file_path)
        self.assertTrue(os.path.isdir(self.output_file_path),
                        "Could not create output file directory")

    def assertFilesEqual(self, base, out):
        """check files are equal"""
        self.assertTrue(os.path.isfile(base), "Base file '{}' does not exist".format(base))
        self.assertTrue(os.path.isfile(out), "Out file '{}' does not exist".format(out))

        with open(base, 'r') as b:
            base_file = ''.join(b.readlines())

        with open(out, 'r') as o:
            out_file = ''.join(o.readlines())

        self.assertEqual(base_file, out_file)

    def test_stamp_fill_embankment(self):
        """Test stamp fill embankment"""
        io = xmsstamper_py.stamper.XmStamperIo()

        # Type and centerline
        io.stamping_type = 1
        io.centerline = ((0, 0, 15), (0, 10, 15))

        # Cross sections
        cs1 = xmsstamper_py.stamper.XmStampCrossSection()
        cs1.left = ((0, 15), (5, 15), (6, 14))
        cs1.left_max = 20
        cs1.index_left_shoulder = 1
        cs1.right = ((0, 15), (5, 15), (6, 14))
        cs1.right_max = 20
        cs1.index_right_shoulder = 1
        cs2 = xmsstamper_py.stamper.XmStampCrossSection()
        cs2.left = ((0, 15), (5, 15), (6, 14))
        cs2.left_max = 20
        cs2.index_left_shoulder = 1
        cs2.right = ((0, 15), (5, 15), (6, 14))
        cs2.right_max = 20
        cs2.index_right_shoulder = 1
        io.cs = (cs1, cs2)

        # Raster to stamp
        no_data = -9999999
        num_pixels_x = 41
        num_pixels_y = 11
        pixel_size = 1
        min_pt = (-20.0, 0.0)
        raster_vals = [no_data] * (num_pixels_x * num_pixels_y)
        raster = xmsstamper_py.stamper.XmStampRaster()
        raster.no_data = no_data
        raster.num_pixels_x = num_pixels_x
        raster.num_pixels_y = num_pixels_y
        raster.pixel_size_x = pixel_size
        raster.pixel_size_y = pixel_size
        raster.min = min_pt
        raster.vals = raster_vals
        io.raster = raster

        st = xmsstamper_py.stamper.XmStamper()
        st.do_stamp(io)

        base_file = os.path.join(
            self.base_file_path, "stamping", "rasterTestFiles", 
            "testFillEmbankment_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "raster_stamp_fill_embankment_out.asc"
        )

        raster_format_enum = xmsstamper_py.stamper.XmStampRaster.raster_format_enum
        io.raster.write_grid_file(output_file, raster_format_enum.RS_ARCINFO_ASCII)

        # Verify grid file
        self.assertFilesEqual(base_file, output_file)

        # Verify outputs
        self.assertEqual(7, len(io.breaklines))

        breaklines = [
            (0, 1), (3, 2, 0, 6, 7), (5, 4, 1, 8, 9), (3, 5), (7, 9), 
            (2, 4), (6, 8)
        ]

        for i in range(0, len(breaklines)):
            self.assertEqual(io.breaklines[i], breaklines[i])

    def test_stamp_cut_embankment(self):
        """Test stamp cut embankment"""
        io = xmsstamper_py.stamper.XmStamperIo()

        # Type and centerline
        io.stamping_type = 1
        io.centerline = ((0, 0, 0), (0, 10, 0))

        # Cross sections
        cs1 = xmsstamper_py.stamper.XmStampCrossSection()
        cs1.left = ((0, 0), (5, 0), (6, 1))
        cs1.left_max = 20
        cs1.index_left_shoulder = 1
        cs1.right = ((0, 0), (5, 0), (6, 1))
        cs1.right_max = 20
        cs1.index_right_shoulder = 1
        cs2 = xmsstamper_py.stamper.XmStampCrossSection()
        cs2.left = ((0, 0), (5, 0), (6, 1))
        cs2.left_max = 20
        cs2.index_left_shoulder = 1
        cs2.right = ((0, 0), (5, 0), (6, 1))
        cs2.right_max = 20
        cs2.index_right_shoulder = 1
        io.cs = (cs1, cs2)

        # Raster to stamp
        no_data = -9999999
        num_pixels_x = 41
        num_pixels_y = 11
        pixel_size = 1
        min_pt = (-20.0, 0.0)
        raster_vals = [no_data] * (num_pixels_x * num_pixels_y)
        raster = xmsstamper_py.stamper.XmStampRaster()
        raster.no_data = no_data
        raster.num_pixels_x = num_pixels_x
        raster.num_pixels_y = num_pixels_y
        raster.pixel_size_x = pixel_size
        raster.pixel_size_y = pixel_size
        raster.min = min_pt
        raster.vals = raster_vals
        io.raster = raster

        st = xmsstamper_py.stamper.XmStamper()
        st.do_stamp(io)

        base_file = os.path.join(
            self.base_file_path, "stamping", "rasterTestFiles", 
            "testCutEmbankment_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "raster_stamp_cut_embankment_out.asc"
        )

        raster_format_enum = xmsstamper_py.stamper.XmStampRaster.raster_format_enum
        io.raster.write_grid_file(output_file, raster_format_enum.RS_ARCINFO_ASCII)

        # Verify grid file
        self.assertFilesEqual(base_file, output_file)

        # Verify outputs
        self.assertEqual(7, len(io.breaklines))

        breaklines = [
            (0, 1), (3, 2, 0, 6, 7), (5, 4, 1, 8, 9), (3, 5), (7, 9), 
            (2, 4), (6, 8)
        ]

        for i in range(0, len(breaklines)):
            self.assertEqual(io.breaklines[i], breaklines[i])

    def test_stamp_wing_wall(self):
        """Test stamp wing wall"""
        io = xmsstamper_py.stamper.XmStamperIo()

        # Type and centerline
        io.stamping_type = 1
        io.centerline = ((0, 0, 15), (20, 20, 15))

        # Cross sections
        cs1 = xmsstamper_py.stamper.XmStampCrossSection()
        cs1.left = ((0, 15), (5, 15), (6, 14))
        cs1.left_max = 20
        cs1.index_left_shoulder = 1
        cs1.right = ((0, 15), (5, 15), (6, 14))
        cs1.right_max = 20
        cs1.index_right_shoulder = 1
        cs2 = xmsstamper_py.stamper.XmStampCrossSection()
        cs2.left = ((0, 15), (5, 15), (6, 14))
        cs2.left_max = 20
        cs2.index_left_shoulder = 1
        cs2.right = ((0, 15), (5, 15), (6, 14))
        cs2.right_max = 20
        cs2.index_right_shoulder = 1
        io.cs = (cs1, cs2)

        io.first_end_cap.angle = 15
        io.first_end_cap.wing_wall.wing_wall_angle = 15

        io.last_end_cap.angle = -15
        io.last_end_cap.wing_wall.wing_wall_angle = 10

        # Raster to stamp
        no_data = -9999999
        num_pixels_x = 46
        num_pixels_y = 45
        pixel_size = 1
        min_pt = (-16.0, -8.0)
        raster_vals = [no_data] * (num_pixels_x * num_pixels_y)
        raster = xmsstamper_py.stamper.XmStampRaster()
        raster.no_data = no_data
        raster.num_pixels_x = num_pixels_x
        raster.num_pixels_y = num_pixels_y
        raster.pixel_size_x = pixel_size
        raster.pixel_size_y = pixel_size
        raster.min = min_pt
        raster.vals = raster_vals
        io.raster = raster

        st = xmsstamper_py.stamper.XmStamper()
        st.do_stamp(io)

        base_file = os.path.join(
            self.base_file_path, "stamping", "rasterTestFiles", 
            "testWingWall_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "raster_wing_wall_out.asc"
        )

        raster_format_enum = xmsstamper_py.stamper.XmStampRaster.raster_format_enum
        io.raster.write_grid_file(output_file, raster_format_enum.RS_ARCINFO_ASCII)

        # Verify grid file
        self.assertFilesEqual(base_file, output_file)

        # Verify outputs
        self.assertEqual(7, len(io.breaklines))

        breaklines = [
            (0, 1), (3, 2, 0, 6, 7), (5, 4, 1, 8, 9), (3, 5), (7, 9),
            (2, 4), (6, 8)
        ]

        for i in range(0, len(breaklines)):
            self.assertEqual(io.breaklines[i], breaklines[i])

    def test_stamp_sloped_abutment(self):
        """Test stamp sloped abutment"""
        io = xmsstamper_py.stamper.XmStamperIo()

        # Type and centerline
        io.stamping_type = 1
        io.centerline = ((0, 0, 15), (20, 20, 15))

        # Cross sections
        cs1 = xmsstamper_py.stamper.XmStampCrossSection()
        cs1.left = ((0, 15), (5, 15), (6, 14))
        cs1.left_max = 20
        cs1.index_left_shoulder = 1
        cs1.right = ((0, 15), (5, 15), (6, 14))
        cs1.right_max = 20
        cs1.index_right_shoulder = 1
        cs2 = xmsstamper_py.stamper.XmStampCrossSection()
        cs2.left = ((0, 15), (5, 15), (6, 14))
        cs2.left_max = 20
        cs2.index_left_shoulder = 1
        cs2.right = ((0, 15), (5, 15), (6, 14))
        cs2.right_max = 20
        cs2.index_right_shoulder = 1
        io.cs = (cs1, cs2)

        # Change to sloped abutment end cap
        io.first_end_cap.type = 1

        # Set the slope and max length for the end cap
        io.first_end_cap.sloped_abutment.slope = ((0, 15), (1, 14))
        io.first_end_cap.sloped_abutment.max_x = 10

        # last end cap
        io.last_end_cap.type = 1
        io.last_end_cap.sloped_abutment.slope = ((0, 15), (1, 14))
        io.last_end_cap.sloped_abutment.max_x = 10  #TODO: Hangs when this is not set
        io.last_end_cap.angle = 20

        # Raster to stamp
        no_data = -9999999
        num_pixels_x = 56
        num_pixels_y = 53
        pixel_size = 1
        min_pt = (-17.0, -17.0)
        raster_vals = [no_data] * (num_pixels_x * num_pixels_y)
        raster = xmsstamper_py.stamper.XmStampRaster()
        raster.no_data = no_data
        raster.num_pixels_x = num_pixels_x
        raster.num_pixels_y = num_pixels_y
        raster.pixel_size_x = pixel_size
        raster.pixel_size_y = pixel_size
        raster.min = min_pt
        raster.vals = raster_vals
        io.raster = raster

        st = xmsstamper_py.stamper.XmStamper()
        st.do_stamp(io)

        base_file = os.path.join(
            self.base_file_path, "stamping", "rasterTestFiles", 
            "testSlopedAbutment_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "test_sloped_abutment_out.asc"
        )

        raster_format_enum = xmsstamper_py.stamper.XmStampRaster.raster_format_enum
        io.raster.write_grid_file(output_file, raster_format_enum.RS_ARCINFO_ASCII)

        # Verify grid file
        self.assertFilesEqual(base_file, output_file)

        base_pts = ((0, 0, 15), (20, 20, 15), (-3.5, 3.5, 15), (-14.1, 14.1, 0),
                    (15.2, 22.2, 15), (4.6, 32.9, 0), (3.5, -3.5, 15),
                    (14.1, -14.1, 0), (24.8, 17.8, 15), (35.4, 7.1, 0),
                    (-10.6, -3.5, 5), (-12.9, -1.9, 4.2), (-14.8, 0.5, 3.3),
                    (-16, 3.5, 2.5), (-16.4, 7, 1.7), (-15.8, 10.6, 0.8),
                    (-3.5, -10.6, 5.0), (-1.9, -12.9, 4.2), (0.5, -14.8, 3.3),
                    (3.5, -16.0, 2.5), (7.0, -16.4, 1.7), (10.6, -15.8, 0.8),
                    (19.4, 31.3, 5), (16.6, 33.4, 3.8), (13, 34.6, 2.5),
                    (8.8, 34.4, 1.3), (29, 26.8, 5), (31.8, 25.9, 4.3),
                    (34.3, 24.1, 3.6), (36.4, 21.5, 2.9), (37.7, 18.2, 2.1),
                    (38, 14.6, 1.4), (37.3, 10.8, 0.7))

        np.testing.assert_array_almost_equal(base_pts, io.get_out_tin().pts, 
                                             decimal=1)

    def test_stamp_guide_bank(self):
        """Test stamp guidebank"""
        io = xmsstamper_py.stamper.XmStamperIo()

        # Type and centerline
        io.stamping_type = 1
        io.centerline = ((0, 0, 15), (50, 50, 15))

        # Cross sections
        cs1 = xmsstamper_py.stamper.XmStampCrossSection()
        cs1.left = ((0, 15), (5, 15), (6, 14))
        cs1.left_max = 10
        cs1.index_left_shoulder = 1
        cs1.right = ((0, 15), (5, 15), (6, 14))
        cs1.right_max = 10
        cs1.index_right_shoulder = 1
        cs2 = xmsstamper_py.stamper.XmStampCrossSection()
        cs2.left = ((0, 15), (5, 15), (6, 14))
        cs2.left_max = 10
        cs2.index_left_shoulder = 1
        cs2.right = ((0, 15), (5, 15), (6, 14))
        cs2.right_max = 10
        cs2.index_right_shoulder = 1
        io.cs = (cs1, cs2)

        # change to guidebank end cap
        io.first_end_cap.type = 0
        io.first_end_cap.guidebank.n_pts = 10
        io.first_end_cap.guidebank.radius1 = 30
        io.first_end_cap.guidebank.radius2 = 15
        io.first_end_cap.guidebank.side = 0 # Left side
        io.first_end_cap.guidebank.width = 6

        io.last_end_cap.type = 0
        io.last_end_cap.guidebank.n_pts = 10
        io.last_end_cap.guidebank.radius1 = 30
        io.last_end_cap.guidebank.radius2 = 15
        io.last_end_cap.guidebank.side = 0 # Left side
        io.last_end_cap.guidebank.width = 6

        io.last_end_cap.angle = 10

        # Raster to stamp
        no_data = -9999999
        num_pixels_x = 90
        num_pixels_y = 81
        pixel_size = 1
        min_pt = (-21.0, -12.0)
        raster_vals = [no_data] * (num_pixels_x * num_pixels_y)
        raster = xmsstamper_py.stamper.XmStampRaster()
        raster.no_data = no_data
        raster.num_pixels_x = num_pixels_x
        raster.num_pixels_y = num_pixels_y
        raster.pixel_size_x = pixel_size
        raster.pixel_size_y = pixel_size
        raster.min = min_pt
        raster.vals = raster_vals
        io.raster = raster

        st = xmsstamper_py.stamper.XmStamper()
        st.do_stamp(io)

        base_file = os.path.join(
            self.base_file_path, "stamping", "rasterTestFiles", 
            "testGuidebank_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "test_guidebank_out.asc"
        )

        raster_format_enum = xmsstamper_py.stamper.XmStampRaster.raster_format_enum
        io.raster.write_grid_file(output_file, raster_format_enum.RS_ARCINFO_ASCII)

        # Verify grid file
        self.assertFilesEqual(base_file, output_file)

    def test_stamp_intersect_bathymetry(self):
        """Test stamp guidebank"""
        io = xmsstamper_py.stamper.XmStamperIo()

        # Type and centerline
        io.stamping_type = 1
        io.centerline = ((0, 0, 15), (10, 10, 15))

        # Cross sections
        cs1 = xmsstamper_py.stamper.XmStampCrossSection()
        cs1.left = ((0, 15), (5, 15), (6, 14))
        cs1.left_max = 20
        cs1.index_left_shoulder = 1
        cs1.right = ((0, 15), (5, 15), (6, 14))
        cs1.right_max = 20
        cs1.index_right_shoulder = 1
        cs2 = xmsstamper_py.stamper.XmStampCrossSection()
        cs2.left = ((0, 15), (5, 15), (6, 14))
        cs2.left_max = 20
        cs2.index_left_shoulder = 1
        cs2.right = ((0, 15), (5, 15), (6, 14))
        cs2.right_max = 20
        cs2.index_right_shoulder = 1
        io.cs = (cs1, cs2)

        # Create a TIN to represent bathymetry
        tin = xmsinterp_py.triangulate.TrTin()
        pts = ((-1, 25, 6), (-15, 11, 6), (5, -11, 10), (20, 4, 10))
        tris = (0, 1, 2, 1, 3, 2)
        tin.set_points(pts)
        tin.set_triangles(tris)
        io.bathymetry = tin

        # Raster to stamp
        no_data = -9999999
        num_pixels_x = 29
        num_pixels_y = 34
        pixel_size = 1
        min_pt = (-10.0, -8.0)
        raster_vals = [no_data] * (num_pixels_x * num_pixels_y)
        raster = xmsstamper_py.stamper.XmStampRaster()
        raster.no_data = no_data
        raster.num_pixels_x = num_pixels_x
        raster.num_pixels_y = num_pixels_y
        raster.pixel_size_x = pixel_size
        raster.pixel_size_y = pixel_size
        raster.min = min_pt
        raster.vals = raster_vals
        io.raster = raster

        st = xmsstamper_py.stamper.XmStamper()
        st.do_stamp(io)

        base_file = os.path.join(
            self.base_file_path, "stamping", "rasterTestFiles", 
            "testIntersectBathymetry_base.asc"
        )
        output_file = os.path.join(
            self.output_file_path, "test_intersect_bathymetry_base.asc"
        )

        raster_format_enum = xmsstamper_py.stamper.XmStampRaster.raster_format_enum
        io.raster.write_grid_file(output_file, raster_format_enum.RS_ARCINFO_ASCII)

        # Verify grid file
        self.assertFilesEqual(base_file, output_file)

        base_pts = ((0, 0, 15), (10, 10, 15), (-3.54, 3.54, 15),
                    (-9.42, 9.42, 6.68), (6.46, 13.54, 15), (-4.14, 24.14, 0),
                    (3.54, -3.54, 15), (7.18, -7.18, 9.84), (13.54, 6.46, 15),
                    (17.18, 2.82, 9.84))

        np.testing.assert_array_almost_equal(base_pts, io.get_out_tin().pts,
                                             decimal=2)