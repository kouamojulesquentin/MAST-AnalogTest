//===========================================================================
//                           MastEnvironment.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file MastEnvironment.cpp
//!
//! Implements class MastEnvironment
//!
//===========================================================================

#include "MastEnvironment.hpp"
#include "MastEnvironment_impl.hpp"

#include <fstream>

using std::vector;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;

using namespace mast;

MastEnvironment::~MastEnvironment ()
{
}

MastEnvironment::MastEnvironment (bool unitTestContext)
  : m_impl(make_unique<MastEnvironment_impl>(unitTestContext))
{
}

void MastEnvironment::CreateApplications ()
{
  m_impl->CreateApplications();
}


void MastEnvironment::CreateManager ()
{
  m_impl->CreateManager();
}

void MastEnvironment::CreateSystemModel ()
{
  m_impl->CreateSystemModel();
}


void MastEnvironment::LoadPlugins ()
{
  m_impl->LoadPlugins();
}

bool MastEnvironment::ParseOptions (int argc, const char* argv[])
{
  return m_impl->ParseOptions(argc, argv);
}


bool MastEnvironment::ParseOptions (vector<string> arguments)
{
  return m_impl->ParseOptions(arguments);
}


void MastEnvironment::Start (int argc, const char* argv[])
{
  m_impl->Start(argc, argv);
}

void MastEnvironment::Start (vector<string> arguments)
{
  m_impl->Start(arguments);
}

void MastEnvironment::Start ()
{
  m_impl->Start();
}

//===========================================================================
// End of MastEnvironment.cpp
//===========================================================================
