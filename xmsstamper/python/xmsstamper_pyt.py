"""Test xmsstamper.cpp"""
import unittest
import xmsstamper


class TestStamper(unittest.TestCase):
    """Tests for the main module"""
    def test_package_docstring(self):
        """Test package has docstring"""
        doc_string = xmsstamper.__doc__
        self.assertIsNotNone(doc_string)
        self.assertGreater(len(doc_string), 0)
