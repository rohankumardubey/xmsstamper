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
                return PyIterFromVecDbl(self.m_distance);
            },
            [](xms::XmStamperCenterlineProfile &self, py::iterable distance)
			{
                self.m_distance = VecDblFromPyIter(distance);
            },
            distance_doc
        )
  // ---------------------------------------------------------------------------
  // function: elevation
  // ---------------------------------------------------------------------------
  const char* elevation_doc = R"pydoc(
      Elevation at the cross section location
  )pydoc";
  stamper_centerline_profile.def_property("elevation",
            [](xms::XmStamperCenterlineProfile &self) -> py::iterable
			{
                return PyIterFromVecDbl(self.m_elevation);
            },
            [](xms::XmStamperCenterlineProfile &self, py::iterable elevation)
			{
                self.m_elevation = VecDblFromPyIter(elevation);
            },
            elevation_doc
        )

  py::class_<xms::XmStamperIo, boost::shared_ptr<xms::XmStamperIo>> stamper_io(m, "XmStamperIo");
  stamper_io.def(py::init<>());
  // ---------------------------------------------------------------------------
  // function: set_centerline
  // ---------------------------------------------------------------------------
  const char* set_centerline_doc = R"pydoc(
      Center line for the feature stamp (Required)

      Args:
          centerline (VecPt3d): The centerline points.
  )pydoc";
  stamper_io.def("set_centerline",
  [](xms::XmStamper &self, py::iterable centerline)
  {
    self.m_centerLine = xms::VecPt3dFromPyIter(centerline);
  },
  set_centerline_doc,
  py::arg("centerline"));
  // ---------------------------------------------------------------------------
  // function: set_stamping_type
  // ---------------------------------------------------------------------------
  const char* set_stamping_type_doc = R"pydoc(
      Sets the stamping type.  0 - Cut, 1 - Fill

      Args:
          stamping_type (int): The stamping type
  )pydoc";
  stamper_io.def("set_stamping_type",
  [](xms::XmStamper &self, int stamping_type)
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
  // function: set_bathymetry
  // ---------------------------------------------------------------------------
  const char* set_bathymetry_doc = R"pydoc(
      Sets underlying bathymetry

      Args:
          bathymetry (TrTin): The bathymetry data
  )pydoc";
  stamper_io.def("set_bathymetry",
  [](xms::XmStamper &self, xms::TrTin &bathymetry)
  {
    self.m_bathemetry = bathymetry;
  },
  set_bathymetry_doc,
  py::arg("bathymetry"));
  // ---------------------------------------------------------------------------
  // function: get_out_tin
  // ---------------------------------------------------------------------------
  const char* get_out_tin_doc = R"pydoc(
      Gets the TIN created by the stamp operation

      Return:
          The output TIN
  )pydoc";
  stamper_io.def("get_out_tin",
  [](xms::XmStamper &self) -> xms::TrTin
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
  [](xms::XmStamper &self) -> py::iterable
  {
    return xms::PyIterFromVecInt2d(self.m_outBreakLines);
  },
  get_breaklines_doc);
}
