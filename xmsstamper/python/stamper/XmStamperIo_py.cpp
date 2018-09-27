//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <iostream>
#include <sstream>
#include <boost/shared_ptr.hpp> // boost::shared_ptr
#include <xmsstamper/stamper/XmStamperIo.h>
#include <xmscore/python/misc/PyUtils.h> // PyIterFromVecPt3d, etc.
#include <xmsinterp/triangulate/TrTin.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initXmStamperIo(py::module &m)
{
// -----------------------------------------------------------------------------
// XMWINGWALL
// -----------------------------------------------------------------------------
  py::class_<xms::XmWingWall, boost::shared_ptr<xms::XmWingWall>> 
    stamper_wing_wall(m, "XmWingWall");
  stamper_wing_wall.def(py::init<>());
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

// -----------------------------------------------------------------------------
// XMSLOPEDABUTMENT
// -----------------------------------------------------------------------------
  py::class_<xms::XmSlopedAbutment, boost::shared_ptr<xms::XmSlopedAbutment>>
    stamper_sloped_abutment(m, "XmSlopedAbutment");
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
    return xms::PyIterFromVecPt2d(self.m_slope);
  },
    [](xms::XmSlopedAbutment &self, py::iterable slope)
  {
    self.m_slope = *xms::VecPt2dFromPyIter(slope);
  },
  slope_doc);
  stamper_sloped_abutment.def("__str__", [](xms::XmSlopedAbutment &self) {
    std::stringstream ss;
    ss << "max_x: " << self.m_maxX << std::endl;
    ss << "slope: " << self.m_slope << std::endl;
    return ss.str();
  });

// -----------------------------------------------------------------------------
// XMGUIDEBANK
// -----------------------------------------------------------------------------
  py::class_<xms::XmGuidebank, boost::shared_ptr<xms::XmGuidebank>>
    stamper_guide_bank(m, "XmGuidebank");
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

// -----------------------------------------------------------------------------
// XMSTAMPERENDCAP
// -----------------------------------------------------------------------------
  py::class_<xms::XmStamperEndCap, boost::shared_ptr<xms::XmStamperEndCap>>
    stamper_end_cap(m, "XmStamperEndCap");
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

// -----------------------------------------------------------------------------
// XMSTAMPCROSSSECTION
// -----------------------------------------------------------------------------
  py::class_<xms::XmStampCrossSection, 
    boost::shared_ptr<xms::XmStampCrossSection>> 
    stamper_cross_section(m, "XmStampCrossSection");
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
                return xms::PyIterFromVecPt2d(self.m_left);
            },
            [](xms::XmStampCrossSection &self, py::iterable left)
            {
                self.m_left = *xms::VecPt2dFromPyIter(left);
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
                return xms::PyIterFromVecPt2d(self.m_right);
            },
            [](xms::XmStampCrossSection &self, py::iterable right)
            {
                self.m_right = *xms::VecPt2dFromPyIter(right);
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



// -----------------------------------------------------------------------------
// XMSTAMPERCENTERLINEPROFILE
// -----------------------------------------------------------------------------
  py::class_<xms::XmStamperCenterlineProfile, 
    boost::shared_ptr<xms::XmStamperCenterlineProfile>> 
    stamper_centerline_profile(m, "XmStamperCenterlineProfile");
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
      std::cout << "cs: " << i << std::endl;
      auto cs = item;
      vec_cs.at(i) = item.cast<xms::XmStampCrossSection>();
      i++;
    }
    self.m_cs = vec_cs;
  },
  cs_centerline_doc);

// -----------------------------------------------------------------------------
// XMSTAMPERIO
// -----------------------------------------------------------------------------
  py::class_<xms::XmStamperIo, boost::shared_ptr<xms::XmStamperIo>> 
    stamper_io(m, "XmStamperIo");
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
      std::cout << "cs: " << i << std::endl;
      auto cs = item;
      vec_cs.at(i) = item.cast<xms::XmStampCrossSection>();
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
  // property: bathymetry
  // ---------------------------------------------------------------------------
  const char* bathymetry_doc = R"pydoc(
      Underlying bathymetry
  )pydoc";
  stamper_io.def_property("bathymetry",
  [](xms::XmStamperIo &self) -> boost::shared_ptr<xms::TrTin>
  {
    return self.m_bathemetry;
  }, 
  [](xms::XmStamperIo &self, boost::shared_ptr<xms::TrTin> &bathymetry)
  {
    self.m_bathemetry = bathymetry;
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

  // TODO: Get rid of this.
  stamper_io.def("from_string",
  &xms::XmStamperIo::FromString, 
  "from string docs");
  stamper_io.def("__str__",
  &xms::XmStamperIo::ToString,
  "to string docs");
}
