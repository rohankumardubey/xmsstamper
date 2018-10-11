"""Test xmsstamper_py.cpp"""
import unittest
import xmsstamper_py


class TestXmStamper(unittest.TestCase):
    """Tests for the main module"""
    def test_package_docstring(self):
        """Test package has docstring"""
        doc_string = xmsstamper_py.__doc__
        self.assertIsNotNone(doc_string)
        self.assertIsInstance(doc_string, str)
        self.assertGreater(len(doc_string), 0)
