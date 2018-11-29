//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <sstream>

#include <pybind11/pybind11.h>

#include <xmscore/stl/vector.h>
#include <xmscore/misc/StringUtil.h>

#include <xmsstamper/python/stamper/stamper_py.h>
#include <xmsstamper/stamper/XmStamperIo.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------

void initStamper(py::module &m) {
  initXmStamper(m);
  initXmStamperIo(m);
}
//------------------------------------------------------------------------------
/// \brief Create string from XmStampRaster
/// \param[in] a_stampRaster: XmStampRaster object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmStampRaster(const xms::XmStampRaster& a_stampRaster)
{
  std::stringstream ss;
  ss << "num_pixels_x: " << a_stampRaster.m_numPixelsX << "\n";
  ss << "num_pixels_y: " << a_stampRaster.m_numPixelsY << "\n";
  ss << "pixel_size_x: " << a_stampRaster.m_pixelSizeX << "\n";
  ss << "pixel_size_y: " << a_stampRaster.m_pixelSizeY << "\n";
  ss << "min: (" << a_stampRaster.m_min.x << ", " << a_stampRaster.m_min.y << ")\n";
  ss << "vals: " << StringFromVecDbl(a_stampRaster.m_vals) << "\n";
  ss << "no_data: " << a_stampRaster.m_noData;
  return ss.str();
} // PyReprStringFromXmStampRaster
//------------------------------------------------------------------------------
/// \brief Create string from XmWingWall
/// \param[in] a_stampRaster: XmWingWall object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmWingWall(const xms::XmWingWall& a_wingWall)
{
  std::stringstream ss;
  ss << "wing_wall_angle: " << a_wingWall.m_wingWallAngle;
  return ss.str();
} // PyReprStringFromXmWingWall
//------------------------------------------------------------------------------
/// \brief Create string from XmSlopedAbutment
/// \param[in] a_stampRaster: XmSlopedAbutment object
/// \return a string
//------------------------------------------------------------------------------
std::string PyReprStringFromXmSlopedAbutment(const xms::XmSlopedAbutment& a_slopedAbutment)
{
  std::stringstream ss;

  return ss.str();
} // PyReprStringFromXmSlopedAbutment
//------------------------------------------------------------------------------
/// \brief Create a __repr__ string from a VecDbl
/// \param[in] a_vals: VecDbl
/// \return a string
//------------------------------------------------------------------------------
std::string StringFromVecDbl(const std::vector<double>& a_vals)
{
  std::stringstream ss;
  ss << "(length " << a_vals.size() << "): ";
  std::vector<double> v;
  bool printDotDotDot(false);
  if (a_vals.size() < 11)
  {
    v = a_vals;
  }
  else
  {
    printDotDotDot = true;
    size_t idx = a_vals.size();
    v.insert(v.end(), a_vals.begin(), a_vals.begin()+5);
    v.insert(v.end(), a_vals.begin()+(idx-5), a_vals.end());
  }

  ss << "[";
  for (size_t i = 0; i < v.size(); ++i)
  {
    if (printDotDotDot && i == 5)
      ss << "... ";
    ss << xms::STRstd(v[i]);
    if (i + 1 != v.size())
      ss << ", ";
  }
  ss << "]\n";
  return ss.str();
} // StringFromDynBitset