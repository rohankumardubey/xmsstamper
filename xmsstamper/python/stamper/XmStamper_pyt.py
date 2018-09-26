"""Test XmStamper_py.cpp"""
import unittest
import xmsstamper_py
import xmsinterp_py

class TestXmStamper(unittest.TestCase):
    """XmStamper tests"""
    def do_stamp_test(self):
        """Stamp to a very basic TIN with 2 triangles"""
		tin = xmsinterp_py.triangulate.TrTin()
		# points in no particular order
		pts = ((0, 0, 0), (1, 0, 0), (0, 1, 0), (1, 1, 0))
		# triangles in CCW order
		tris = (2, 0, 1, 1, 3, 2)
		# triangles adjacent to each point (why can't we get this from the above 2 lists?)
        tris_adj = ((0), (0, 1), (0, 1), (2))
		tin.set_geometry(pts, tris, tris_adj)
		stampio = xmsstamper_py.stamper.XmStamperIo()
		stampio.set_bathymetry(tin)
		stamppts = ((0, 0, 0), (1, 1, 0))
		stampio.set_centerline(stamppts)
		stamp = xmsstamper_py.stamper.XmStamper()
		stamp.do_stamp(stampio)
		outTin = stampio.get_out_tin()
		print(outTin)
		
    def test_tutorial(self):
        """A test to serve as a tutorial"""
