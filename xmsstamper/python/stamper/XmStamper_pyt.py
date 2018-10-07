"""Test XmStamper_py.cpp"""
import unittest
import os
import xmsstamper_py
import xmsinterp_py

class TestXmStamper(unittest.TestCase):
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
        print(self.output_file_path)
        self.assertTrue(os.path.isdir(self.output_file_path), 
            "Could not create output file directory")

    def assertFilesEqual(self, base, out):
        self.assertTrue(os.path.isfile(base), "Base file does not exist")
        self.assertTrue(os.path.isfile(out), "Out file does not exist")

        with open(base, 'r') as b:
            base_file = ''.join(b.readlines())

        with open(out, 'r') as o:
            out_file = ''.join(o.readlines())

        self.assertEqual(base_file, out_file)


    """XmStamper tests"""        
    def test_stamp_fill_embankment(self):
        """Test stamp file embankment"""
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
            self.output_file_path, "rastest_stamp_fill_embankmentter.asc"
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
