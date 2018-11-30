//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/shared_ptr.hpp> // boost::shared_ptr
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <xmscore/misc/xmstype.h> // XM_NODATA
#include <xmscore/python/misc/PyUtils.h> // PyIterFromVecPt3d, etc.

#include <xmsinterp/triangulate/TrTin.h>

#include <xmsstamper/python/stamper/stamper_py.h>
#include <xmsstamper/stamper/XmStamperIo.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initXmStamperIo(py::module &m)
{
// -----------------------------------------------------------------------------
// XMSTAMPRASTER
// -----------------------------------------------------------------------------
  py::class_<xms::XmStampRaster, boost::shared_ptr<xms::XmStampRaster>>
    stamp_raster(m, "StampRaster");

  const char* stamper_raster_init_doc = R"pydoc(
       StampRaster Initializer

       Args:
           num_pixels_x (int): Number of pixes in the X-direction
           num_pixels_y (int): Number of pixes in the X-direction
           pixel_size_x (float): Number of pixes in the X-direction
           pixel_size_y (float): Number of pixes in the X-direction
           min (iterable): Number of pixes in the X-direction
           vals (iterable): Number of pixes in the X-direction
           no_data (int): Number of pixes in the X-direction
  )pydoc";
  stamp_raster.def(py::init<>([](std::string file_name) {
    boost::shared_ptr<xms::XmStampRaster> xm_stamp_raster(new xms::XmStampRaster());
    std::ifstream is;
    is.open(file_name, std::ios::in);
    xm_stamp_raster->ReadFromFile(is);
    is.close();
    return xm_stamp_raster;
  }), stamper_raster_init_doc, py::arg("file_name");
  stamp_raster.def(py::init<>([](py::object num_pixels_x, py::object num_pixels_y,
           py::object pixel_size_x, py::object pixel_size_y, py::object min,
           py::object vals, py::object no_data) {
    boost::shared_ptr<xms::XmStampRaster> xm_stamp_raster(new xms::XmStampRaster());
    if (!num_pixels_x.is_none())
    {
      xm_stamp_raster->m_numPixelsX = py::cast<int>(num_pixels_x);
    }
    if (!num_pixels_y.is_none())
    {
      xm_stamp_raster->m_numPixelsY = py::cast<int>(num_pixels_y);
    }
    if (!pixel_size_x.is_none())
    {
      xm_stamp_raster->m_pixelSizeX = py::cast<double>(pixel_size_x);
    }
    if (!pixel_size_y.is_none())
    {
      xm_stamp_raster->m_pixelSizeY = py::cast<double>(pixel_size_y);
    }
    if (!min.is_none())
    {
      xm_stamp_raster->m_min = xms::Pt3dFromPyIter(min);
    }
    if (!vals.is_none())
    {
      xm_stamp_raster->m_vals = *xms::VecDblFromPyIter(vals);
    }
    if (!no_data.is_none())
    {
      xm_stamp_raster->m_noData = py::cast<int>(no_data);
    }
    return xm_stamp_raster;
  }), stamper_raster_init_doc,
  py::arg("num_pixels_x") = py::none(), py::arg("num_pixels_y") = py::none(),
  py::arg("pixel_size_x") = py::none(), py::arg("pixel_size_y") = py::none(),
  py::arg("min") = py::none(), py::arg("vals") = py::none(),
  py::arg("no_data") = py::none());
  // ---------------------------------------------------------------------------
  // property: num_pixels_x
  // ---------------------------------------------------------------------------
  const char* num_pixels_x_doc = R"pydoc(
       Number of pixels in the X-direction (Required)
  )pydoc";
  stamp_raster.def_readwrite("num_pixels_x",
    &xms::XmStampRaster::m_numPixelsX, num_pixels_x_doc);
  // ---------------------------------------------------------------------------
  // property: num_pixels_y
  // ---------------------------------------------------------------------------
  const char* num_pixels_y_doc = R"pydoc(
       Number of pixels in the Y-direction (Required)
  )pydoc";
  stamp_raster.def_readwrite("num_pixels_y",
    &xms::XmStampRaster::m_numPixelsY, num_pixels_y_doc);
  // ---------------------------------------------------------------------------
  // property: pixel_size_x
  // ---------------------------------------------------------------------------
  const char* pixel_size_x_doc = R"pydoc(
       Pixel size in the X-direction (Required)
  )pydoc";
  stamp_raster.def_readwrite("pixel_size_x",
    &xms::XmStampRaster::m_pixelSizeX, pixel_size_x_doc);
  // ---------------------------------------------------------------------------
  // property: pixel_size_y
  // ---------------------------------------------------------------------------
  const char* pixel_size_y_doc = R"pydoc(
       Pixel size in the Y-direction (Required)
  )pydoc";
  stamp_raster.def_readwrite("pixel_size_y",
    &xms::XmStampRaster::m_pixelSizeY, pixel_size_y_doc);
  // ---------------------------------------------------------------------------
  // property: no_data
  // ---------------------------------------------------------------------------
  const char* no_data_doc = R"pydoc(
       NO DATA value for the raster
  )pydoc";
  stamp_raster.def_readwrite("no_data",
    &xms::XmStampRaster::m_noData, no_data_doc);
  // ---------------------------------------------------------------------------
  // property: min
  // ---------------------------------------------------------------------------
  const char* min_doc = R"pydoc(
       Minimum (lower left) X, Y coordinate of the raster at the center of the 
       raster cell (Required)
  )pydoc";
  stamp_raster.def_property("min",
  [](xms::XmStampRaster &self) -> py::tuple 
  {
    return xms::PyIterFromPt3d(self.m_min);
  },
  [](xms::XmStampRaster &self, py::tuple _min)
  {
    self.m_min = xms::Pt3dFromPyIter(_min);
  }
  , min_doc);
  // ---------------------------------------------------------------------------
  // property: min
  // ---------------------------------------------------------------------------
  const char* vals_doc = R"pydoc(
        Raster values defined from the top left corner to the bottom right 
        corner (Required). Use the m_noData value to specify a cell value with 
        no data.
  )pydoc";
  stamp_raster.def_property("vals",
    [](xms::XmStampRaster &self) -> py::iterable
  {
    return xms::PyIterFromVecDbl(self.m_vals);
  },
    [](xms::XmStampRaster &self, py::iterable _vals)
  {
    self.m_vals = *xms::VecDblFromPyIter(_vals);
  }
  , vals_doc);
  // ---------------------------------------------------------------------------
  // function: get_cell_index_from_col_row
  // ---------------------------------------------------------------------------
  const char* get_cell_index_from_col_row_raster_doc = R"pydoc(
      Gets the zero-based cell index from the given column and row.
      
      Args:
          col (int): The zero-based column index for the raster.
          row (int): The zero-based row index for the raster.

      Returns:
          The zero-based cell index from the given a_col, a_row.
  )pydoc";
  stamp_raster.def("get_cell_index_from_col_row",
    &xms::XmStampRaster::GetCellIndexFromColRow,
    get_cell_index_from_col_row_raster_doc, py::arg("col"), py::arg("row"));
  // ---------------------------------------------------------------------------
  // function: get_col_row_from_cell_index
  // ---------------------------------------------------------------------------
  const char* get_col_row_from_cell_index_doc = R"pydoc(
      Gets the zero-based column and row from the cell index.
      
      Args:
          index (int): The zero-based raster cell index.

      Returns:
          Tuple of (col, row) of the cell by index
  )pydoc";
  stamp_raster.def("get_col_row_from_cell_index",
  [](xms::XmStampRaster &self, int index)
  {
    int c, r;
    self.GetColRowFromCellIndex(index, c, r);
    return py::make_tuple(c, r);
  },
  get_col_row_from_cell_index_doc, py::arg("int"));
  // ---------------------------------------------------------------------------
  // function: get_location_from_cell_index
  // ---------------------------------------------------------------------------
  const char* get_location_from_cell_index_doc = R"pydoc(
      Gets the location of the cell center from the zero-based cell index.
      
      Args:
          index (int): The zero-based raster cell index.

      Returns:
          The location of the cell with the given index.
  )pydoc";
  stamp_raster.def("get_location_from_cell_index",
  [](xms::XmStampRaster &self, int index)
  {
    return xms::PyIterFromPt3d(self.GetLocationFromCellIndex(index));
  },
  get_location_from_cell_index_doc, py::arg("index"));
  // ---------------------------------------------------------------------------
  // function: write_grid_file
  // ---------------------------------------------------------------------------
  const char* write_grid_file_doc = R"pydoc(
      Writes the raster in the given format to the given filename.
      
      Args:
         file_name (str): The output raster filename.
         format (): The output raster format
  )pydoc";
  stamp_raster.def("write_grid_file",
    &xms::XmStampRaster::WriteGridFile,
    write_grid_file_doc, py::arg("file_name"), py::arg("format"));
  // ---------------------------------------------------------------------------
  // function: read_from_file
  // ---------------------------------------------------------------------------
  const char* read_from_file_raster_doc = R"pydoc(
      Reads the XmStampRaster class information from a file.
      
      Args:
         file_name (str): The input file.
  )pydoc";
  stamp_raster.def("read_from_file",
    [](xms::XmStampRaster &self, std::string file_name)
  {
    std::ifstream is;
    is.open(file_name, std::ios::in);
    self.ReadFromFile(is);
    is.close();
  }
    ,
    read_from_file_raster_doc, py::arg("file_name"));
  // ---------------------------------------------------------------------------
  // function: write_to_file
  // ---------------------------------------------------------------------------
  const char* write_to_file_raster_doc = R"pydoc(
      Writes the XmStampRaster class information to a file.
      
      Args:
         file_name (str): The input file.
         card_name (str): The card name to be written to the output file.
  )pydoc";
  stamp_raster.def("write_to_file",
    [](xms::XmStampRaster &self, std::string file_name, std::string card_name)
  {
    // TODO: This needs some error checking
    std::ofstream os;
    os.open(file_name, std::ios::out);
    self.WriteToFile(os, card_name);
    os.close();
  }
    ,
    write_to_file_raster_doc, py::arg("file_name"), py::arg("card_name"));
  // -------------------------------------------------------------------------
  // function: __repr__
  // -------------------------------------------------------------------------
  stamp_raster.def("__repr__", [](xms::XmStampRaster &self) {
    return PyReprStringFromXmStampRaster(self);
  });
// -----------------------------------------------------------------------------
// XMSTAMPRASTERENUM
// -----------------------------------------------------------------------------
  py::enum_<xms::XmStampRaster::XmRasterFormatEnum>(stamp_raster, 
         "raster_format_enum", "weight_enum for InterpIdw class")
    .value("RS_ARCINFO_ASCII", 
                 xms::XmStampRaster::XmRasterFormatEnum::RS_ARCINFO_ASCII)
    .export_values();

// -----------------------------------------------------------------------------
// XMWINGWALL
// -----------------------------------------------------------------------------
  py::class_<xms::XmWingWall, boost::shared_ptr<xms::XmWingWall>> 
    stamper_wing_wall(m, "WingWall");


  const char* init_doc = R"pydoc(
       WingWall Initializer

       Args:
           wing_wall_angle (float): Wing wall angle
  )pydoc";
  stamper_wing_wall.def(py::init<>([](py::object wing_wall_angle) {
    boost::shared_ptr<xms::XmWingWall> rval(new xms::XmWingWall());
    if (!wing_wall_angle.is_none())
    {
      rval->m_wingWallAngle = py::cast<double>(wing_wall_angle);
    }
    return rval;
  }), init_doc, py::arg("wing_wall_angle") = py::none());
  // ---------------------------------------------------------------------------
  // property: wing_wall_angle
  // ---------------------------------------------------------------------------
  const char* wing_wall_angle_doc = R"pydoc(
       Degrees from 0 to 60
  )pydoc";
  stamper_wing_wall.def_readwrite("wing_wall_angle", 
             &xms::XmWingWall::m_wingWallAngle, wing_wall_angle_doc);
  stamper_wing_wall.def("__str__", [](xms::XmWingWall &self) {
    std::stringstream ss;
    ss << "wing_wall_angle: " << self.m_wingWallAngle << std::endl;
    return ss.str();
  });
  // -------------------------------------------------------------------------
  // function: __repr__
  // -------------------------------------------------------------------------
  stamper_wing_wall.def("__repr__", [](xms::XmWingWall &self) {
    return PyReprStringFromXmWingWall(self);
  });

// -----------------------------------------------------------------------------
// XMSLOPEDABUTMENT
// -----------------------------------------------------------------------------
  py::class_<xms::XmSlopedAbutment, boost::shared_ptr<xms::XmSlopedAbutment>>
    stamper_sloped_abutment(m, "SlopedAbutment");
  stamper_sloped_abutment.def(py::init<>());
  // ---------------------------------------------------------------------------
  // property: max_x
  // ---------------------------------------------------------------------------
  const char* max_x_doc = R"pydoc(
       Max distance from center line
  )pydoc";
  stamper_sloped_abutment.def_readwrite("max_x", 
    &xms::XmSlopedAbutment::m_maxX, max_x_doc);
  // ---------------------------------------------------------------------------
  // property: slope
  // ---------------------------------------------------------------------------
  const char* slope_doc = R"pydoc(
      x,y pairs defining slope from center line
  )pydoc";
  stamper_sloped_abutment.def_property("slope",
    [](xms::XmSlopedAbutment &self) -> py::iterable
  {
    return xms::PyIterFromVecPt3d(self.m_slope);
  },
    [](xms::XmSlopedAbutment &self, py::iterable slope)
  {
    self.m_slope = *xms::VecPt3dFromPyIter(slope);
  },
  slope_doc);
  stamper_sloped_abutment.def("__str__", [](xms::XmSlopedAbutment &self) {
    std::stringstream ss;
    ss << "max_x: " << self.m_maxX << std::endl;
    ss << "slope: " << self.m_slope << std::endl;
    return ss.str();
  });
  // -------------------------------------------------------------------------
  // function: __repr__
  // -------------------------------------------------------------------------
  stamper_sloped_abutment.def("__repr__", [](xms::XmSlopedAbutment &self) {
    return PyReprStringFromXmSlopedAbutment(self);
  });

// -----------------------------------------------------------------------------
// XMGUIDEBANK
// -----------------------------------------------------------------------------
  py::class_<xms::XmGuidebank, boost::shared_ptr<xms::XmGuidebank>>
    stamper_guide_bank(m, "Guidebank");
  stamper_guide_bank.def(py::init<>());
  // ---------------------------------------------------------------------------
  // property: side
  // ---------------------------------------------------------------------------
  const char* side_doc = R"pydoc(
       Position of guidebank relative to center line, 0-left, 1-right
  )pydoc";
  stamper_guide_bank.def_readwrite("side",
    &xms::XmGuidebank::m_side, side_doc);
  // ---------------------------------------------------------------------------
  // property: radius1
  // ---------------------------------------------------------------------------
  const char* radius1_doc = R"pydoc(
       First radius (R1) for guidebank creation
  )pydoc";
  stamper_guide_bank.def_readwrite("radius1",
    &xms::XmGuidebank::m_radius1, radius1_doc);
  // ---------------------------------------------------------------------------
  // property: m_radius2
  // ---------------------------------------------------------------------------
  const char* radius2_doc = R"pydoc(
       Second radius (R2) for guidebank creation
  )pydoc";
  stamper_guide_bank.def_readwrite("radius2",
    &xms::XmGuidebank::m_radius2, radius2_doc);
  // ---------------------------------------------------------------------------
  // property: m_width
  // ---------------------------------------------------------------------------
  const char* width_doc = R"pydoc(
       Width of guidebank about the center line
  )pydoc";
  stamper_guide_bank.def_readwrite("width",
    &xms::XmGuidebank::m_width, width_doc);
  // ---------------------------------------------------------------------------
  // property: n_pts
  // ---------------------------------------------------------------------------
  const char* n_pts_doc = R"pydoc(
       n_pts of guidebank about the center line
  )pydoc";
  stamper_guide_bank.def_readwrite("n_pts",
    &xms::XmGuidebank::m_nPts, n_pts_doc);
  stamper_guide_bank.def("__str__", [](xms::XmGuidebank &self) {
    std::stringstream ss;
    ss << "side: " << self.m_side << std::endl;
    ss << "radius1: " << self.m_radius1 << std::endl;
    ss << "radius2: " << self.m_radius2 << std::endl;
    ss << "width: " << self.m_width << std::endl;
    ss << "n_pts: " << self.m_nPts << std::endl;
    return ss.str();
  });
  // -------------------------------------------------------------------------
  // function: __repr__
  // -------------------------------------------------------------------------
  stamper_guide_bank.def("__repr__", [](xms::XmGuidebank &self) {
    return PyReprStringFromXmGuidebank(self);
  });

// -----------------------------------------------------------------------------
// XMSTAMPERENDCAP
// -----------------------------------------------------------------------------
  py::class_<xms::XmStamperEndCap, boost::shared_ptr<xms::XmStamperEndCap>>
    stamper_end_cap(m, "StamperEndCap");
  stamper_end_cap.def(py::init<>());
  // ---------------------------------------------------------------------------
  // property: type
  // ---------------------------------------------------------------------------
  const char* type_doc = R"pydoc(
       Type of end cap: 0- guidebank, 1- sloped abutment, 2- wing wall
  )pydoc";
  stamper_end_cap.def_readwrite("type",
    &xms::XmStamperEndCap::m_type, type_doc);
  // ---------------------------------------------------------------------------
  // property: angle
  // ---------------------------------------------------------------------------
  const char* angle_doc = R"pydoc(
       Degrees from -45 to 45
  )pydoc";
  stamper_end_cap.def_readwrite("angle",
    &xms::XmStamperEndCap::m_angle, angle_doc);
  // ---------------------------------------------------------------------------
  // property: guidebank
  // ---------------------------------------------------------------------------
  const char* guidebank_doc = R"pydoc(
       Guidebank definition
  )pydoc";
  stamper_end_cap.def_readwrite("guidebank",
    &xms::XmStamperEndCap::m_guidebank, guidebank_doc);
  // ---------------------------------------------------------------------------
  // property: sloped_abutment
  // ---------------------------------------------------------------------------
  const char* sloped_abutment_doc = R"pydoc(
       Sloped abutment definition
  )pydoc";
  stamper_end_cap.def_readwrite("sloped_abutment",
    &xms::XmStamperEndCap::m_slopedAbutment, sloped_abutment_doc);
  // ---------------------------------------------------------------------------
  // property: wing_wall
  // ---------------------------------------------------------------------------
  const char* wing_wall_doc = R"pydoc(
       Wing wall definition
  )pydoc";
  stamper_end_cap.def_readwrite("wing_wall",
    &xms::XmStamperEndCap::m_wingWall, wing_wall_doc);
  stamper_end_cap.def("__str__", [](xms::XmStamperEndCap &self) {
    std::stringstream ss;
    ss << "type: " << self.m_type << std::endl;
    ss << "angle: " << self.m_angle << std::endl;
    return ss.str();
  });
  // -------------------------------------------------------------------------
  // function: __repr__
  // -------------------------------------------------------------------------
  stamper_end_cap.def("__repr__", [](xms::XmStamperEndCap &self) {
    return PyReprStringFromXmStamperEndCap(self);
  });

// -----------------------------------------------------------------------------
// XMSTAMPCROSSSECTION
// -----------------------------------------------------------------------------
  py::class_<xms::XmStampCrossSection, 
    boost::shared_ptr<xms::XmStampCrossSection>> 
    stamper_cross_section(m, "StampCrossSection");
  stamper_cross_section.def(py::init<>());
  // ---------------------------------------------------------------------------
  // property: left
  // ---------------------------------------------------------------------------
  const char* left_doc = R"pydoc(
      Points defining the cross section
  )pydoc";
  stamper_cross_section.def_property("left",
            [](xms::XmStampCrossSection &self) -> py::iterable
            {
                return xms::PyIterFromVecPt3d(self.m_left);
            },
            [](xms::XmStampCrossSection &self, py::iterable left)
            {
                self.m_left = *xms::VecPt3dFromPyIter(left);
            },
            left_doc
        );
  // ---------------------------------------------------------------------------
  // property: left_max
  // ---------------------------------------------------------------------------
  const char* left_max_doc = R"pydoc(
      Max x value for left side
  )pydoc";
  stamper_cross_section.def_readwrite("left_max", &xms::XmStampCrossSection::m_leftMax,
    left_max_doc);
  // ---------------------------------------------------------------------------
  // property: index_left_shoulder
  // ---------------------------------------------------------------------------
  const char* index_left_shoulder_doc = R"pydoc(
      index to the shoulder point in the left point list
  )pydoc";
  stamper_cross_section.def_readwrite("index_left_shoulder", &xms::XmStampCrossSection::
    m_idxLeftShoulder, index_left_shoulder_doc);
  // ---------------------------------------------------------------------------
  // property: right
  // ---------------------------------------------------------------------------
  const char* right_doc = R"pydoc(
      Points defining the cross section
  )pydoc";
  stamper_cross_section.def_property("right",
            [](xms::XmStampCrossSection &self) -> py::iterable
            {
                return xms::PyIterFromVecPt3d(self.m_right);
            },
            [](xms::XmStampCrossSection &self, py::iterable right)
            {
                self.m_right = *xms::VecPt3dFromPyIter(right);
            },
            right_doc
        );
  // ---------------------------------------------------------------------------
  // property: right_max
  // ---------------------------------------------------------------------------
  const char* right_max_doc = R"pydoc(
      Max x value for right side
  )pydoc";
  stamper_cross_section.def_readwrite("right_max", &xms::XmStampCrossSection::m_rightMax,
    right_max_doc);
  // ---------------------------------------------------------------------------
  // property: index_right_shoulder
  // ---------------------------------------------------------------------------
  const char* index_right_shoulder_doc = R"pydoc(
      index to the shoulder point in the right point list
  )pydoc";
  stamper_cross_section.def_readwrite("index_right_shoulder", &xms::XmStampCrossSection::
    m_idxRightShoulder, index_right_shoulder_doc);
  // -------------------------------------------------------------------------
  // function: __repr__
  // -------------------------------------------------------------------------
  stamper_cross_section.def("__repr__", [](xms::XmStampCrossSection &self) {
    return PyReprStringFromXmStampCrossSection(self);
  });

// -----------------------------------------------------------------------------
// XMSTAMPERCENTERLINEPROFILE
// -----------------------------------------------------------------------------
  py::class_<xms::XmStamperCenterlineProfile, 
    boost::shared_ptr<xms::XmStamperCenterlineProfile>> 
    stamper_centerline_profile(m, "StamperCenterlineProfile");
  stamper_centerline_profile.def(py::init<>());
  // ---------------------------------------------------------------------------
  // property: distance
  // ---------------------------------------------------------------------------
  const char* distance_doc = R"pydoc(
      Distance from start of polyline for cross section
  )pydoc";
  stamper_centerline_profile.def_property("distance",
            [](xms::XmStamperCenterlineProfile &self) -> py::iterable
            {
                return xms::PyIterFromVecDbl(self.m_distance);
            },
            [](xms::XmStamperCenterlineProfile &self, py::iterable distance)
            {
                self.m_distance = *xms::VecDblFromPyIter(distance);
            },
            distance_doc
        );
  // ---------------------------------------------------------------------------
  // property: elevation
  // ---------------------------------------------------------------------------
  const char* elevation_doc = R"pydoc(
      Elevation at the cross section location
  )pydoc";
  stamper_centerline_profile.def_property("elevation",
  [](xms::XmStamperCenterlineProfile &self) -> py::iterable
  {
      return xms::PyIterFromVecDbl(self.m_elevation);
  },
  [](xms::XmStamperCenterlineProfile &self, py::iterable elevation)
  {
      self.m_elevation = *xms::VecDblFromPyIter(elevation);
  },
  elevation_doc);
  // ---------------------------------------------------------------------------
  // function: cs
  // ---------------------------------------------------------------------------
  const char* cs_centerline_doc = R"pydoc(
    Cross sections along the polyLine
  )pydoc";
  stamper_centerline_profile.def_property("cs",
    [](xms::XmStamperCenterlineProfile &self) -> py::iterable
  {
    auto tuple_ret = py::tuple(self.m_cs.size());
    for (size_t i = 0; i < tuple_ret.size(); i++) {
      tuple_ret[i] = self.m_cs.at(i);
    }
    return tuple_ret;
  },
    [](xms::XmStamperCenterlineProfile &self, py::iterable& py_cs)
  {
    std::vector<xms::XmStampCrossSection> vec_cs;
    vec_cs.resize(py::len(py_cs));
    int i = 0;
    for (auto item : py_cs)
    {
      auto cs = item;
      vec_cs.at(i) = item.cast<xms::XmStampCrossSection>();
      i++;
    }
    self.m_cs = vec_cs;
  },
  cs_centerline_doc);
  // -------------------------------------------------------------------------
  // function: __repr__
  // -------------------------------------------------------------------------
  stamper_centerline_profile.def("__repr__", [](xms::XmStamperCenterlineProfile &self) {
    return PyReprStringFromXmStamperCenterlineProfile(self);
  });

// -----------------------------------------------------------------------------
// XMSTAMPERIO
// -----------------------------------------------------------------------------
  py::class_<xms::XmStamperIo, boost::shared_ptr<xms::XmStamperIo>> 
    stamper_io(m, "StamperIo");
  stamper_io.def(py::init<>());
  // ---------------------------------------------------------------------------
  // property: centerline
  // ---------------------------------------------------------------------------
  const char* centerline_doc = R"pydoc(
      Center line for the feature stamp (Required)
  )pydoc";
  stamper_io.def_property("centerline",
  [](xms::XmStamperIo &self) -> py::iterable
  {
  return xms::PyIterFromVecPt3d(self.m_centerLine);
  },
  [](xms::XmStamperIo &self, py::iterable centerline)
  {
    self.m_centerLine = *xms::VecPt3dFromPyIter(centerline);
  },
  centerline_doc);
  // ---------------------------------------------------------------------------
  // property: stamping_type
  // ---------------------------------------------------------------------------
  const char* stamping_type_doc = R"pydoc(
      The stamping type.  0 - Cut, 1 - Fill
  )pydoc";
  stamper_io.def_readwrite("stamping_type", &xms::XmStamperIo::m_stampingType,
    stamping_type_doc
  );
  // ---------------------------------------------------------------------------
  // property: cs
  // ---------------------------------------------------------------------------
  const char* cs_io_doc = R"pydoc(
    Cross sections along the polyLine
  )pydoc";
  stamper_io.def_property("cs",
    [](xms::XmStamperIo &self) -> py::iterable
  {
    auto tuple_ret = py::tuple(self.m_cs.size());
    for (size_t i = 0; i < tuple_ret.size(); i++) {
      tuple_ret[i] = self.m_cs.at(i);
    }
    return tuple_ret;
  },
  [](xms::XmStamperIo &self, py::iterable& py_cs)
  {
    std::vector<xms::XmStampCrossSection> vec_cs;
    vec_cs.resize(py::len(py_cs));
    int i = 0;
    for (auto item : py_cs)
    {
        if (!item.is_none())
        {
            vec_cs.at(i) = item.cast<xms::XmStampCrossSection>();
        }
        i++;
    }
    self.m_cs = vec_cs;
  },
  cs_io_doc);
  // ---------------------------------------------------------------------------
  // property: first_end_cap
  // ---------------------------------------------------------------------------
  const char* first_end_cap_doc = R"pydoc(
      end cap at beginnig of polyline
  )pydoc";
  stamper_io.def_readwrite("first_end_cap", &xms::XmStamperIo::m_firstEndCap,
  first_end_cap_doc);
  // ---------------------------------------------------------------------------
  // property: last_end_cap
  // ---------------------------------------------------------------------------
  const char* last_end_cap_doc = R"pydoc(
      end cap at end of polyline
  )pydoc";
  stamper_io.def_readwrite("last_end_cap", &xms::XmStamperIo::m_lastEndCap,
    last_end_cap_doc);
  // ---------------------------------------------------------------------------
  // property: last_end_cap
  // ---------------------------------------------------------------------------
  const char* raster_doc = R"pydoc(
      Input/output raster to stamp the resulting elevations onto this raster
  )pydoc";
  stamper_io.def_readwrite("raster", &xms::XmStamperIo::m_raster,
    raster_doc);
  // ---------------------------------------------------------------------------
  // property: bathymetry
  // ---------------------------------------------------------------------------
  const char* bathymetry_doc = R"pydoc(
      Underlying bathymetry
  )pydoc";
  stamper_io.def_property("bathymetry",
  [](xms::XmStamperIo &self) -> boost::shared_ptr<xms::TrTin>
  {
    return self.m_bathymetry;
  }, 
  [](xms::XmStamperIo &self, boost::shared_ptr<xms::TrTin> &bathymetry)
  {
    self.m_bathymetry = bathymetry;
  },
  bathymetry_doc);
  // ---------------------------------------------------------------------------
  // function: get_out_tin
  // ---------------------------------------------------------------------------
  const char* get_out_tin_doc = R"pydoc(
      Gets the TIN created by the stamp operation

      Return:
          The output TIN
  )pydoc";
  stamper_io.def("get_out_tin",
  [](xms::XmStamperIo &self) -> boost::shared_ptr<xms::TrTin>
  {
    return self.m_outTin;
  },
  get_out_tin_doc);
  // ---------------------------------------------------------------------------
  // property: breaklines
  // ---------------------------------------------------------------------------
  const char* breaklines_doc = R"pydoc(
      Breaklines that are honored in the TIN
  )pydoc";
  stamper_io.def_property("breaklines",
  [](xms::XmStamperIo &self) -> py::iterable
  {
    return xms::PyIterFromVecInt2d(self.m_outBreakLines);
  },
  [](xms::XmStamperIo &self, py::iterable breaklines)
  {
    self.m_outBreakLines = *xms::VecInt2dFromPyIter(breaklines);
  },
  breaklines_doc);
  // ---------------------------------------------------------------------------
  // function: read_from_file
  // ---------------------------------------------------------------------------
  const char* read_from_file_doc = R"pydoc(
      Reads the XmStamperIo class information from a file.
      
      Args:
         file_name (str): The input file.
  )pydoc";
  stamper_io.def("read_from_file",
  [](xms::XmStamperIo &self, std::string file_name)
  {
    std::ifstream is;
    is.open(file_name, std::ios::in);
    self.ReadFromFile(is);
    is.close();
  }
  ,
  read_from_file_doc, py::arg("file_name"));
  // ---------------------------------------------------------------------------
  // function: write_to_file
  // ---------------------------------------------------------------------------
  const char* write_to_file_doc = R"pydoc(
      Writes the XmStamperIo class information to a file.
      
      Args:
         file_name (str): The input file.
         card_name (str): The card name to be written to the output file.
  )pydoc";
  stamper_io.def("write_to_file",
  [](xms::XmStamperIo &self, std::string file_name, std::string card_name)
  {
    // TODO: This needs some error checking
    std::ofstream os;
    os.open(file_name, std::ios::out);
    self.WriteToFile(os, card_name);
    os.close();
  }
  ,
  write_to_file_doc, py::arg("file_name"), py::arg("card_name"));
  // -------------------------------------------------------------------------
  // function: __repr__
  // -------------------------------------------------------------------------
  stamper_io.def("__repr__", [](xms::XmStamperIo &self) {
    return PyReprStringFromXmStamperIo(self);
  });
}
