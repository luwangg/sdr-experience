/* -*- c++ -*- */
/*
 * Copyright 2004 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#ifndef INCLUDED_HOWTO_SPECTRUM_SENSING_CF_H
#define INCLUDED_HOWTO_SPECTRUM_SENSING_CF_H

#include <gr_sync_decimator.h>
#include <gr_sync_block.h>
#include <gr_sync_interpolator.h>
#include <gr_sync_block.h>
#include <howto_api.h>
#include<boost/math/distributions.hpp>

class howto_spectrum_sensing_cf;

/*
 * We use boost::shared_ptr's instead of raw pointers for all access
 * to gr_blocks (and many other data structures).  The shared_ptr gets
 * us transparent reference counting, which greatly simplifies storage
 * management issues.  This is especially helpful in our hybrid
 * C++ / Python system.
 *
 * See http://www.boost.org/libs/smart_ptr/smart_ptr.htm
 *
 * As a convention, the _sptr suffix indicates a boost::shared_ptr
 */
typedef boost::shared_ptr<howto_spectrum_sensing_cf> howto_spectrum_sensing_cf_sptr;

/*!
 * \brief Return a shared_ptr to a new instance of howto_square_ff.
 *
 * To avoid accidental use of raw pointers, howto_square_ff's
 * constructor is private.  howto_make_square_ff is the public
 * interface for creating new instances.
 */
HOWTO_API howto_spectrum_sensing_cf_sptr howto_make_spectrum_sensing_cf (float sample_rate, int ninput_samples, int samples_per_band, float pfd, float pfa, float tcme, bool output_far, bool debug_stats, int band_location, float useless_band, bool debug_histogram, int nframes_to_check, int nframes_to_average, int downconverter, int nsegs_to_check);

class HOWTO_API howto_spectrum_sensing_cf : public gr_sync_decimator
{
private:
  // The friend declaration allows howto_make_spectrum_sensing_cf to
  // access the private constructor.
  friend HOWTO_API howto_spectrum_sensing_cf_sptr howto_make_spectrum_sensing_cf (float sample_rate, int ninput_samples, int samples_per_band, float pfd, float pfa, float tcme, bool output_far, bool debug_stats, int band_location, float useless_band, bool debug_histogram, int nframes_to_check, int nframes_to_average, int downconverter, int nsegs_to_check);

  float d_sample_rate, d_pfd, d_pfa, d_tcme, d_useless_band;
  float *segment, *sorted_segment;
  int d_ninput_samples, d_samples_per_band, d_band_location, d_useless_segment, d_usefull_samples, d_nsub_bands, *fa_histogram, d_decimation, d_nconsecutive_frames_to_check, d_nframes_to_average, d_downconverter, d_nSegsToCheck;
  bool d_output_far, d_debug_stats, d_debug_histogram;
  gr_complex *new_in;

  howto_spectrum_sensing_cf (float sample_rate, int ninput_samples, int samples_per_band, float pfd, float pfa, float tcme, bool output_far, bool debug_stats, int band_location, float useless_band, bool debug_histogram, int nframes_to_check, int nframes_to_average, int downconverter, int nsegs_to_check);  	// private constructor

  void average_and_segment_spectrum_e4k(const gr_complex *in, int output_item, int segment_group);
  void average_and_segment_spectrum_r820t(const gr_complex *in, int output_item, int segment_group);
  bool sort_energy();
  float calculate_noise_reference(int* n_zref_segs);
  float calculate_scale_factor(int x);
  float calculate_false_alarm_rate(float alpha, float zref, int I);
  float calculate_primary_user_detection_rate(float alpha, float zref, int I);

  void (howto_spectrum_sensing_cf::*avg_and_seg_spectrum)(const gr_complex *in, int output_item, int segment_group);
  int select_avg_and_seg_method(int downconverter, int useless_segment);

 public:
  ~howto_spectrum_sensing_cf ();	// public destructor

  int ninput_samples () const { return d_ninput_samples; }
  void set_ninput_samples (int k) { d_ninput_samples = k; }
	
  float sample_rate() const {return d_sample_rate;}
  void set_sample_rate(float rate) {d_sample_rate = rate;}

  int samples_per_band() const {return d_samples_per_band;}
  void set_samples_per_band(int nsamples) {d_samples_per_band = nsamples;}

  float pfd() const {return d_pfd;}
  void set_pfd(float p) {d_pfd = p;}

  float pfa() const {return d_pfa;}
  void set_pfa(float p) {d_pfa = p;}
	
  float tcme() const {return d_tcme;}
  void set_tcme(float tcme) {d_tcme = tcme;}

  float useless_band() const {return d_useless_band;}
  void set_useless_band(float p) {d_useless_band = p;}

  float band_location() const {return d_band_location;}
  void set_band_location(float band_location) {d_band_location = band_location;}

  bool debug_histogram() const {return d_debug_histogram;}
  void set_debug_histogram(bool hist) {d_debug_histogram = hist;}

  int get_histogram(int pos) {
      return fa_histogram[pos];
   }

   int getNumberOfSubBands() {
      return d_nsub_bands;
   }

  // Where all the action really happens
  int work (int noutput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items);
};

#endif /* INCLUDED_HOWTO_SPECTRUM_SENSING_CF_H */
