#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief
/// \ingroup stamping
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

#ifdef CXX_TEST

// 3. Standard Library Headers
#include <boost/shared_ptr.hpp>

// 4. External Library Headers
#include <cxxtest/TestSuite.h>

// 5. Shared Headers

// 6. Non-shared Headers

namespace xms
{
class TrTin;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief Tests the StStampEndCap class
class XmBathymetryIntersectorUnitTests : public CxxTest::TestSuite
{
public:
  void testCreateClass();
  void testIntersectCenterLine();
  void testIntersectXsects();
  void testClassifyPoints();
  void testDescomposeCenterLine();
}; // XmBathymetryIntersectorUnitTests

//----- Global functions -------------------------------------------------------

boost::shared_ptr<xms::TrTin> trBuildTin();

#endif
