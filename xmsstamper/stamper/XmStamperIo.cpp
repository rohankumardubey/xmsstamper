//------------------------------------------------------------------------------
/// \file
/// \ingroup stamping
/// \brief Implements serialization of the data classes in XmStamperIo.h
//
/// \copyright (C) Copyright Aquaveo 2018.
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsstamper/stamper/XmStamperIo.h>

// 3. Standard library headers
#include <fstream> // std::ofstream
#include <sstream> // std::stringstream

// 4. External library headers
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

// 5. Shared code headers
#include <xmscore/misc/XmError.h> // XM_ENSURE_TRUE
#include <xmscore/misc/XmsType.h> // XM_NODATA
#include <xmsinterp/triangulate/TrTin.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

//------------------------------------------------------------------------------
/// \brief Constructor that sets all the raster values
//------------------------------------------------------------------------------
XmStampRaster::XmStampRaster(const int a_numPixelsX, const int a_numPixelsY, const double a_pixelSizeX,
  const double a_pixelSizeY, const Pt2d &a_min, const std::vector<double> &a_vals, const int a_noData)
  : m_numPixelsX(a_numPixelsX)
  , m_numPixelsY(a_numPixelsY)
  , m_pixelSizeX(a_pixelSizeX)
  , m_pixelSizeY(a_pixelSizeY)
  , m_min(a_min)
  , m_vals(a_vals)
  , m_noData(a_noData)
{
} // XmStampRaster::XmStampRaster
//------------------------------------------------------------------------------
/// \brief Default Constructor
//------------------------------------------------------------------------------
XmStampRaster::XmStampRaster()
  : m_numPixelsX(0)
  , m_numPixelsY(0)
  , m_pixelSizeX(0.0)
  , m_pixelSizeY(0.0)
  , m_min()
  , m_vals()
  , m_noData(XM_NODATA)
{
} // XmStampRaster::XmStampRaster
//------------------------------------------------------------------------------
/// \brief Gets the zero-based cell index from the given column and row.
/// \param[in] a_col: The zero-based column index for the raster.
/// \param[in] a_row: The zero-based row index for the raster.
/// \return The zero-based cell index from the given a_col, a_row.
//------------------------------------------------------------------------------
int XmStampRaster::GetCellIndexFromColRow(const int a_col, const int a_row) const
{
  if (a_col >= 0 && a_col < m_numPixelsX && a_row >= 0 && a_row < m_numPixelsY)
  {
    return a_row * m_numPixelsX + a_col;
  }
  return -1;
} // XmStampRaster::GetCellIndexFromColRow
//------------------------------------------------------------------------------
/// \brief Gets the zero-based column and row from the cell index.
/// \param[in] a_index: The zero-based raster cell index.
/// \param[out] a_col: The zero-based column index for the raster.
/// \param[out] a_row: The zero-based row index for the raster.
//------------------------------------------------------------------------------
void XmStampRaster::GetColRowFromCellIndex(const int a_index, int &a_col, int &a_row) const
{
  a_row = a_col = -1;
  if (a_index > 0 && a_index < m_numPixelsX * m_numPixelsY)
  {
    a_col = a_index % m_numPixelsX;
    a_row = a_index / m_numPixelsX;
  }
} // XmStampRaster::GetColRowFromCellIndex
  //------------------------------------------------------------------------------
/// \brief Gets the location of the cell center from the zero-based cell index.
/// \param[in] a_index: The zero-based raster cell index.
/// \return The location of the cell with the given index.
//------------------------------------------------------------------------------
Pt2d XmStampRaster::GetLocationFromCellIndex(const int a_index) const
{
  int col, row;
  GetColRowFromCellIndex(a_index, col, row);
  if (col >= 0 && row >= 0)
  {
    Pt2d loc;
    loc.x = m_min.x + col * m_pixelSizeX;
    loc.y = m_min.y + (m_numPixelsY - 1 - row) * m_pixelSizeY;
    return loc;
  }
  return m_min;
} // XmStampRaster::GetLocationFromCellIndex
//------------------------------------------------------------------------------
/// \brief Writes the raster in the given format to the given filename.
/// \param[in] a_fileName: The output raster filename.
/// \param[in] a_format: The output raster format.
//------------------------------------------------------------------------------
void XmStampRaster::WriteGridFile(const std::string &a_fileName,
  const XmRasterFormatEnum a_format)
{
  std::ofstream outGrid(a_fileName, std::ofstream::trunc);
  XM_ENSURE_TRUE(outGrid.is_open());
  switch (a_format)
  {
    case RS_ARCINFO_ASCII:
      outGrid << "ncols " << m_numPixelsX << std::endl;
      outGrid << "nrows " << m_numPixelsY << std::endl;
      outGrid << "xllcorner " << std::fixed << std::setprecision(13) << m_min.x - m_pixelSizeX / 2.0 << std::endl;
      outGrid << "yllcorner " << m_min.y - m_pixelSizeY / 2.0 << std::endl;
      outGrid << "cellsize " << m_pixelSizeX << std::endl;
      outGrid << "NODATA_value " << m_noData << std::endl;
      outGrid << std::setprecision(2);
      int count = 0;
      for (const double v : m_vals)
      {
        outGrid << v << " ";
        ++count;
        if (count % m_numPixelsX == 0)
          outGrid << std::endl;
      }
      break;
  }
} // XmStampRaster::WriteGridFile
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmStampRaster::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive &m_numPixelsX;
  archive &m_numPixelsY;
  archive &m_pixelSizeX;
  archive &m_pixelSizeY;
  archive &m_min;
  archive &m_vals;
} // XmStampRaster::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmWingWall::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_wingWallAngle;
} // XmWingWall::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmSlopedAbutment::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_maxX;
  archive& m_slope;
} // XmSlopedAbutment::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmGuidebank::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_side;
  archive& m_radius1;
  archive& m_radius2;
  archive& m_width;
  archive& m_nPts;
} // XmGuidebank::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmStamperEndCap::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_type;
  archive& m_angle;
  archive& m_guidebank;
  archive& m_slopedAbutment;
  archive& m_wingWall;
} // XmStamperEndCap::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmStampCrossSection::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_left;
  archive& m_leftMax;
  archive& m_idxLeftShoulder;
  archive& m_right;
  archive& m_rightMax;
  archive& m_idxRightShoulder;
} // XmStampCrossSection::serialize
//------------------------------------------------------------------------------
/// \brief Boost serialize function.
/// \param[in,out] archive: An archive.
/// \param[in] version: The version number.
//------------------------------------------------------------------------------
template <typename Archive>
void XmStamperIo::serialize(Archive& archive, const unsigned int version)
{
  (void)version; // Because Doxygen complained when commented out above.
  archive& m_centerLine;
  archive& m_stampingType;
  archive& m_cs;
  archive& m_firstEndCap;
  archive& m_lastEndCap;
  archive& m_bathymetry;
  archive& m_outTin;
  archive& m_outBreakLines;
} // XmStamperIo::serialize
//------------------------------------------------------------------------------
/// \brief Use boost archive to get the sampling grid as text.
///
/// This function is in this file so it has access to XmStamperIo.
/// \return A string representation of the class.
//------------------------------------------------------------------------------
std::string XmStamperIo::ToString() const
{
  std::stringstream ss;
  {
    boost::archive::text_oarchive oa(ss);
    oa << *this;
  }
  return ss.str();
} // XmStamperIo::ToString
//------------------------------------------------------------------------------
/// \brief Use boost archive to turn the text into a sampling grid.
///
/// This function is in this file so it has access to XmStamperIo.
//------------------------------------------------------------------------------
void XmStamperIo::FromString(const std::string& a_text)
{
  std::stringstream ss(a_text);
  {
    boost::archive::text_iarchive ia(ss);
    ia >> *this;
  }
} // XmStamperIo::FromString

} // namespace xms
