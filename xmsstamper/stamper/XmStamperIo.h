#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup stamping
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard library headers

// 4. External library headers
#include <xmscore/stl/vector.h>
#include <xmscore/misc/boost_defines.h>

// 5. Shared code headers

//----- Forward declarations ---------------------------------------------------
namespace boost
{
namespace serialization
{
class access;
}
}

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
class TrTin;

////////////////////////////////////////////////////////////////////////////////
/// \class XmStampRaster
/// \brief Raster defined using a non-rotated cartesian axis for use in XmStamper
class XmStampRaster
{
  /// \cond
  friend class boost::serialization::access;
  /// \endcond
public:
  XmStampRaster(const int a_numPixelsX, const int a_numPixelsY, const double a_pixelSizeX,
    const double a_pixelSizeY, const Pt2d &a_min, const std::vector<double> &a_vals, const int a_noData);
  XmStampRaster();
  enum XmRasterFormatEnum {RS_ARCINFO_ASCII};
  int m_numPixelsX; ///< Number of pixels in the X-direction (Required)
  int m_numPixelsY; ///< Number of pixels in the Y-direction (Required)
  double m_pixelSizeX; ///< Pixel size in the X-direction (Required)
  double m_pixelSizeY; ///< Pixel size in the Y-direction (Required)
  Pt2d m_min; ///< Minimum (lower left) X, Y coordinate of the raster at the center of the raster cell (Required)
  std::vector<double> m_vals; ///< Raster values defined from the top left corner to the bottom right corner (Required)
                              ///< Use the m_noData value to specify a cell value with no data.
  int m_noData; ///< NO DATA value for the raster (typically XM_NODATA)
  int GetCellIndexFromColRow(const int a_col, const int a_row) const;
  void GetColRowFromCellIndex(const int a_index, int & a_col, int & a_row) const;
  Pt2d GetLocationFromCellIndex(const int a_index) const;
  void WriteGridFile(const std::string &a_fileName, const XmRasterFormatEnum a_format);
  template <typename Archive>
  void serialize(Archive& archive, const unsigned int version);
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmWingWall
/// \brief Wing wall definition for feature stamp end cap
class XmWingWall
{
  /// \cond
  friend class boost::serialization::access;
  /// \endcond
public:
  XmWingWall()
  : m_wingWallAngle(0)
  {
  }

  double m_wingWallAngle; ///< degrees from 0 to 60

  template <typename Archive>
  void serialize(Archive& archive, const unsigned int version);
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmSlopedAbutment
/// \brief Abutment definition for feature stamp end cap
class XmSlopedAbutment
{
  /// \cond
  friend class boost::serialization::access;
  /// \endcond
public:
  XmSlopedAbutment()
  : m_maxX(0)
  , m_slope()
  {
  }

  double m_maxX;   ///< max distance from center line
  VecPt2d m_slope; ///< x,y pairs defining slope from center line

  template <typename Archive>
  void serialize(Archive& archive, const unsigned int version);
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmGuidebank
/// \brief Guidebank definition for feature stamp end cap
class XmGuidebank
{
  /// \cond
  friend class boost::serialization::access;
  /// \endcond
public:
  XmGuidebank()
  : m_side(0)
  , m_radius1(0)
  , m_radius2(0)
  , m_width(0)
  , m_nPts(10)
  {
  }

  int m_side;       ///< position of guidebank relative to center line, 0-left, 1-right
  double m_radius1; ///< first radius (R1) for guidebank creation
  double m_radius2; ///< second radius (R2) for guidebank creation
  double m_width;   ///< width of guidebank about the center line
  int m_nPts;       ///< number of points created along the center line to create the guidebank

  template <typename Archive>
  void serialize(Archive& archive, const unsigned int version);
};
////////////////////////////////////////////////////////////////////////////////
/// \class XmStamperEndCap
/// \brief End cap definition for feature stamp
class XmStamperEndCap
{
  /// \cond
  friend class boost::serialization::access;
  /// \endcond
public:
  XmStamperEndCap()
  : m_type(2)
  , m_angle(0)
  {
  }

  int m_type;              ///< type of end cap: 0- guidebank, 1- sloped abutment, 2- wing wall
  double m_angle;          ///< degrees from -45 to 45
  XmGuidebank m_guidebank; ///< guidebank definition
  XmSlopedAbutment m_slopedAbutment; ///< sloped abutment definition
  XmWingWall m_wingWall;             ///< wing wall definition

  template <typename Archive>
  void serialize(Archive& archive, const unsigned int version);
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmStampCrossSection
/// \brief Cross section definition for stamping
class XmStampCrossSection
{
  /// \cond
  friend class boost::serialization::access;
  /// \endcond
public:
  XmStampCrossSection()
  : m_left()
  , m_leftMax(0.0)
  , m_idxLeftShoulder(0)
  , m_right()
  , m_rightMax(0.0)
  , m_idxRightShoulder(0)
  {
  }

  /// left side of the cross section
  VecPt2d m_left;        ///< points defining the cross section
  double m_leftMax;      ///< max x value for left side
  int m_idxLeftShoulder; ///< index to the shoulder point in the m_left vector
  /// right side of the cross section
  VecPt2d m_right;        ///< points defining the cross section
  double m_rightMax;      ///< max x value for right side
  int m_idxRightShoulder; ///< index to the shoulder point in the m_right vector

  template <typename Archive>
  void serialize(Archive& archive, const unsigned int version);
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmStamperCenterlineProfile
/// \brief Cross section for stamping using distance rather than polyline index
class XmStamperCenterlineProfile
{
public:
  VecDbl m_distance;                     ///< distance from start of polyline for cross section
  VecDbl m_elevation;                    ///< elevation at the cross section location
  std::vector<XmStampCrossSection> m_cs; ///< cross sections at locations along the polyline
};

////////////////////////////////////////////////////////////////////////////////
/// \class XmStamperIo
/// \brief Stamping inputs/outputs class
class XmStamperIo
{
  /// \cond
  friend class boost::serialization::access;
  /// \endcond
public:
  /// \brief Constructor
  XmStamperIo()
  : m_centerLine()
  , m_stampingType(0)
  , m_cs()
  , m_firstEndCap()
  , m_lastEndCap()
  , m_bathymetry()
  , m_outTin()
  , m_outBreakLines()
  {
  }

  /// Input
  /// Required. Center line for the feature stamp
  VecPt3d m_centerLine;
  /// Stamping type 0 - Cut, 1 - Fill
  int m_stampingType;
  /// cross sections along the polyLine
  std::vector<XmStampCrossSection> m_cs;
  /// end cap at beginnig of polyline
  XmStamperEndCap m_firstEndCap;
  /// end cap at end of polyline
  XmStamperEndCap m_lastEndCap;
  /// underlying bathymetry
  BSHP<TrTin> m_bathymetry;

  /// Output
  /// TIN created by the stamp operation
  BSHP<TrTin> m_outTin;
  /// break lines that are honored in the TIN
  VecInt2d m_outBreakLines;
  /// Input/output raster to stamp the resulting elevations onto this raster
  XmStampRaster m_raster;

  std::string ToString() const;
  void FromString(const std::string&);
  template <typename Archive>
  void serialize(Archive& archive, const unsigned int version);
}; // XmStamperIo

//----- Function prototypes ----------------------------------------------------

} // namespace xms
