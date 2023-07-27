/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include <iostream>
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkNullImageToImageFilterDriver.hxx"
#include "itkSimpleFilterWatcher.h"
#include "itkTestingMacros.h"

/**
 * Test the class instance by driving it with a null input and output.
 * Returns 0 on success and 1 on failure.
 */
int
itkCurvatureAnisotropicDiffusionImageFilterTest(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  using ImageType = itk::Image<float, 2>;

  // Set up filter
  itk::CurvatureAnisotropicDiffusionImageFilter<ImageType, ImageType>::Pointer filter =
    itk::CurvatureAnisotropicDiffusionImageFilter<ImageType, ImageType>::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(filter, CurvatureAnisotropicDiffusionImageFilter, AnisotropicDiffusionImageFilter);


  itk::SimpleFilterWatcher watcher(filter);


  itk::IdentifierType numberOfIterations = 1;
  filter->SetNumberOfIterations(numberOfIterations);
  ITK_TEST_SET_GET_VALUE(numberOfIterations, filter->GetNumberOfIterations());

  auto conductanceParameter = 3.0;
  filter->SetConductanceParameter(conductanceParameter);
  ITK_TEST_SET_GET_VALUE(conductanceParameter, filter->GetConductanceParameter());

  FilterType::TimeStepType timeStep = 0.125;
  filter->SetTimeStep(timeStep);
  ITK_TEST_SET_GET_VALUE(timeStep, filter->GetTimeStep());

  // Run Test
  itk::Size<2> sz;
  sz[0] = 250;
  sz[1] = 250;
  itk::NullImageToImageFilterDriver<ImageType, ImageType> test1;
  test1.SetImageSize(sz);
  test1.SetFilter(filter);

  ITK_TRY_EXPECT_NO_EXCEPTION(test1.Execute());


  return EXIT_SUCCESS;
}
