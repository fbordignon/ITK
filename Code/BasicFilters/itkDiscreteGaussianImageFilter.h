/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkDiscreteGaussianImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#ifndef __itkDiscreteGaussianImageFilter_h
#define __itkDiscreteGaussianImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"

namespace itk
{
/**
 * \class DiscreteGaussianImageFilter
 * \brief Blurs an image by separable convolution with discrete gaussian kernels.
 * This filter performs Gaussian blurring by separable convolution of an image
 * and a discrete Gaussian operator (kernel).
 *
 * The Gaussian operator used here was described by Tony Lindeberg (Discrete
 * Scale-Space Theory and the Scale-Space Primal Sketch.  Dissertation. Royal
 * Institute of Technology, Stockholm, Sweden. May 1991.)
 *
 * \sa GaussianOperator
 * \sa Image
 * \sa Neighborhood
 * \sa NeighborhoodOperator
 */

template <class TInputImage, class TOutputImage >
class ITK_EXPORT DiscreteGaussianImageFilter :
    public ImageToImageFilter< TInputImage, TOutputImage > 
{
public:
  /**
   * Standard "Self" & superclass typedef.
   */
  typedef DiscreteGaussianImageFilter Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage > Superclass;

  /**
   * Image type information
   */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /**
   * Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same.
   */
  typedef typename TOutputImage::PixelType OutputPixelType;
  typedef typename TOutputImage::InternalPixelType OutputInternalPixelType;
  typedef typename TInputImage::PixelType InputPixelType;
  typedef typename TInputImage::InternalPixelType InputInternalPixelType;
  enum { ImageDimension = TOutputImage::ImageDimension };
  
  /** 
   * Smart pointer typedef support 
   */
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro(DiscreteGaussianImageFilter, ImageToImageFilter);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /**
   * Standard pipeline method.
   */
  void GenerateData();

  /**
   * Standard get/set macros for filter parameters.
   */
  itkSetVectorMacro(Variance, float, ImageDimension);
  itkGetVectorMacro(Variance, const float, ImageDimension);
  itkSetVectorMacro(MaximumError, float, ImageDimension);
  itkGetVectorMacro(MaximumError, const float, ImageDimension);

  /**
   * Convenience get/set methods for setting all dimensional parameters to the
   * same values.
   */
  void SetVariance(const float v)
  {
    float vArray[ImageDimension];
    for (unsigned int i = 0; i<ImageDimension; ++i) { vArray[i] = v; }
    this->SetVariance(vArray);
  }
  void SetMaximumError(const float v)
  {
    float vArray[ImageDimension];
    for (unsigned int i = 0; i<ImageDimension; ++i) { vArray[i] = v; }
    this->SetMaximumError(vArray);
  }
  
protected:
  DiscreteGaussianImageFilter()
  {
    this->SetVariance(0.0f);
    this->SetMaximumError(0.01f);
  }
  virtual ~DiscreteGaussianImageFilter() {}
  DiscreteGaussianImageFilter(const Self&) {}
  void operator=(const Self&) {}

  static void ImageRegionCopy(OutputImageType *, InputImageType *);

private:
  /**
   * The variance of the gaussian blurring kernel in each dimensional direction.
   */
  float m_Variance[ImageDimension];

  /**
   * The maximum error of the gaussian blurring kernel in each dimensional
   * direction. For definition of maximum error, see GaussianOperator.
   * \sa GaussianOperator
   */
  float m_MaximumError[ImageDimension];  
};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDiscreteGaussianImageFilter.txx"
#endif

#endif
