
#include "FieldGeometry.h"
#include "../Fundamental/stringconvert.h"
#include <cmath>
#include <sstream>

using namespace std;
using namespace Tribots;

Tribots::FieldGeometry::FieldGeometry () noexcept :
    clipping_geometry(NULL) {;}

Tribots::FieldGeometry::~FieldGeometry () noexcept {
   if (clipping_geometry)
     delete clipping_geometry;
}

FieldGeometry::FieldGeometry (const FieldGeometry& fg) noexcept :
    clipping_geometry(NULL) {
  operator= (fg);
}

const FieldGeometry& FieldGeometry::operator= (const FieldGeometry& fg) noexcept {
  if (clipping_geometry) {
    delete clipping_geometry;
    clipping_geometry=NULL;
  }
  field_length=fg.field_length;
  field_width=fg.field_width;
  side_band_width=fg.side_band_width;
  goal_band_width=fg.goal_band_width;
  goal_area_length=fg.goal_area_length;
  goal_area_width=fg.goal_area_width;
  penalty_area_length=fg.penalty_area_length;
  penalty_area_width=fg.penalty_area_width;
  center_circle_radius=fg.center_circle_radius;
  corner_arc_radius=fg.corner_arc_radius;
  penalty_marker_distance=fg.penalty_marker_distance;
  line_thickness=fg.line_thickness;
  border_line_thickness=fg.border_line_thickness;
  goal_width=fg.goal_width;
  goal_length=fg.goal_length;
  goal_height=fg.goal_height;
  pole_height=fg.pole_height;
  pole_diameter=fg.pole_diameter;
  pole_position_offset_x=fg.pole_position_offset_x;
  pole_position_offset_y=fg.pole_position_offset_y;
  ball_diameter=fg.ball_diameter;
  lineset_type=fg.lineset_type;
   
  clipping_geometry=0;
  return *this;
}

bool Tribots::FieldGeometry::operator== (const FieldGeometry& fg) const noexcept {
  if (std::abs(field_length-fg.field_length)>10) return false;
  if (std::abs(field_width-fg.field_width)>10) return false;
  if (std::abs(side_band_width-fg.side_band_width)>10) return false;
  if (std::abs(goal_band_width-fg.goal_band_width)>10) return false;
  if (std::abs(goal_area_length-fg.goal_area_length)>10) return false;
  if (std::abs(goal_area_width-fg.goal_area_width)>10) return false;
  if (std::abs(penalty_area_length-fg.penalty_area_length)>10) return false;
  if (std::abs(penalty_area_width-fg.penalty_area_width)>10) return false;
  if (std::abs(center_circle_radius-fg.center_circle_radius)>10) return false;
  if (std::abs(corner_arc_radius-fg.corner_arc_radius)>10) return false;
  if (std::abs(penalty_marker_distance-fg.penalty_marker_distance)>10) return false;
  if (std::abs(line_thickness-fg.line_thickness)>10) return false;
  if (std::abs(border_line_thickness-fg.border_line_thickness)>10) return false;
  if (std::abs(goal_width-fg.goal_width)>10) return false;
  if (std::abs(goal_length-fg.goal_length)>10) return false;
  if (std::abs(goal_height-fg.goal_height)>10) return false;
  if (std::abs(pole_diameter-fg.pole_diameter)>10) return false;
  if (std::abs(pole_height-fg.pole_height)>10) return false;
  if (std::abs(pole_position_offset_x-fg.pole_position_offset_x)>10) return false;
  if (std::abs(pole_position_offset_y-fg.pole_position_offset_y)>10) return false;
  if (std::abs(ball_diameter-fg.ball_diameter)>10) return false;
  if (lineset_type!=fg.lineset_type) return false;
  return true;
}

bool Tribots::FieldGeometry::operator!= (const FieldGeometry& fg) const noexcept {
  return !operator== (fg);
}


std::string FieldGeometry::serialize () const {
  stringstream inout;
  inout << field_length << ' '
            << field_width << ' '
            << side_band_width << ' '
            << goal_band_width << ' '
            << goal_area_length << ' '
            << goal_area_width << ' '
            << penalty_area_length << ' '
            << penalty_area_width << ' '
            << center_circle_radius << ' '
            << corner_arc_radius << ' '
            << penalty_marker_distance << ' '
            << line_thickness << ' '
            << border_line_thickness << ' '
            << goal_width << ' '
            << goal_length << ' '
            << goal_height << ' '
            << pole_height << ' '
            << pole_diameter << ' '
            << pole_position_offset_x << ' '
            << pole_position_offset_y << ' '
            << ball_diameter << ' '
            << lineset_type;
  
  string result;
  getline (inout, result);
  return result;
}

bool FieldGeometry::deserialize (const std::string& s) noexcept {
  FieldGeometry fg;
  std::vector<std::string> parts;
  Tribots::split_string (parts, s);
  if (parts.size()<22)
    return false;
  string2double (fg.field_length, parts[0]);
  string2double (fg.field_width, parts[1]);
  string2double (fg.side_band_width, parts[2]);
  string2double (fg.goal_band_width, parts[3]);
  string2double (fg.goal_area_length, parts[4]);
  string2double (fg.goal_area_width, parts[5]);
  string2double (fg.penalty_area_length, parts[6]);
  string2double (fg.penalty_area_width, parts[7]);
  string2double (fg.center_circle_radius, parts[8]);
  string2double (fg.corner_arc_radius, parts[9]);
  string2double (fg.penalty_marker_distance, parts[10]);
  string2double (fg.line_thickness, parts[11]);
  string2double (fg.border_line_thickness, parts[12]);
  string2double (fg.goal_width, parts[13]);
  string2double (fg.goal_length, parts[14]);
  string2double (fg.goal_height, parts[15]);
  string2double (fg.pole_height, parts[16]);
  string2double (fg.pole_diameter, parts[17]);
  string2double (fg.pole_position_offset_x, parts[18]);
  string2double (fg.pole_position_offset_y, parts[19]);
  string2double (fg.ball_diameter, parts[20]);
  int lineset=0;
  string2int (lineset, parts[21]);
  fg.lineset_type= FieldGeometry::LinesetType (lineset);
   
  operator= (fg);
  return true;
}
