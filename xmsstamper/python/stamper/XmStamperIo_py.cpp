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
  py::class_<xms::XmStamperCenterlineProfile, boost::shared_ptr<xms::XmStamperCenterlineProfile>> stamper_centerline_profile(m, "XmStamperCenterlineProfile");
  stamper_centerline_profile.def(py::init<>());
  // ---------------------------------------------------------------------------
  // function: distance
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
  // function: elevation
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
            elevation_doc
        );

  py::class_<xms::XmStamperIo, boost::shared_ptr<xms::XmStamperIo>> stamper_io(m, "XmStamperIo");
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
  // function: set_stamping_type
  // ---------------------------------------------------------------------------
  const char* set_stamping_type_doc = R"pydoc(
      Sets the stamping type.  0 - Cut, 1 - Fill

      Args:
          stamping_type (int): The stamping type
  )pydoc";
  stamper_io.def("set_stamping_type",
  [](xms::XmStamperIo &self, int stamping_type)
  {
    self.m_stampingType = stamping_type;
  },
  set_stamping_type_doc,
  py::arg("stamping_type"));
  // ---------------------------------------------------------------------------
  // function: set_cs TO DO
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  // function: set_first_end_cap TO DO
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  // function: set_last_end_cap TO DO
  // ---------------------------------------------------------------------------
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
  // function: get_breaklines
  // ---------------------------------------------------------------------------
  const char* get_breaklines_doc = R"pydoc(
      Gets the breaklines that are honored in the TIN

      Return:
          py::iterable (VecInt2d): indices of breaklines that are honored in the TIN
  )pydoc";
  stamper_io.def("get_breaklines",
  [](xms::XmStamperIo &self) -> py::iterable
  {
    return xms::PyIterFromVecInt2d(self.m_outBreakLines);
  },
  get_breaklines_doc);
}
