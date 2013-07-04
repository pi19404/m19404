#include <ftd/discretizers/gradient_discretizer.h>




// ----------------------------------------------------------------------------
void
ftd::discretizers::
GradientDiscretizer::
discretize (
  IplImage * image,
  const int regionWidth,
  const int regionHeight,
  unsigned char * discretizedData,
  const int numOfGradients )
{
  const int horizontalSamples = image->width/regionWidth;
  const int verticalSamples = image->height/regionHeight;

  // compute gradients
  IplImage * sobelDx = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelDy = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelMagnitude = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelAngle = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);

  cvSobel (image, sobelDx, 1, 0, 3);
  cvSobel (image, sobelDy, 0, 1, 3);

  cvCartToPolar (sobelDx, sobelDy, sobelMagnitude, sobelAngle, 1); // the "1" means that the angles are in degree


  memset (discretizedData, 0, horizontalSamples*verticalSamples*sizeof (unsigned char));

  // discretize
  int offsetX = 1;
  int offsetY = 1;

  const int numOfGradientBins = 8;
  const int samplingSize = 7;
  //const int numOfGradients = 8;
  const float minimumGradientMagnitude = 10.0f;

  const float divisor = 180.0f/(numOfGradientBins-1.0f);

  unsigned char * dataIter = discretizedData;
  for (int rowIndex = 0; rowIndex < verticalSamples; ++rowIndex)
  {
    for (int colIndex = 0; colIndex < horizontalSamples; ++colIndex)
    {
      int counter = 0;

      while (counter < numOfGradients)
      {
        float maxGradient;
        int maxGradientPosX;
        int maxGradientPosY;

        // find next location and value of maximum gradient magnitude in current region
        {
          double minValue;
          double maxValue;
          CvPoint minLocation;
          CvPoint maxLocation;

          cvSetImageROI (sobelMagnitude, cvRect (offsetX, offsetY, samplingSize, samplingSize));
          cvMinMaxLoc (sobelMagnitude, &minValue, &maxValue, &minLocation, &maxLocation);
          maxGradient = static_cast<float> (maxValue);
          maxGradientPosX = maxLocation.x;
          maxGradientPosY = maxLocation.y;
          cvResetImageROI (sobelMagnitude);
        }

        if (maxGradient < minimumGradientMagnitude)
        {
          *dataIter |= 1 << (numOfGradientBins-1);
          break;
        }
        else
        {
          const int angle = static_cast<int> (CV_IMAGE_ELEM (sobelAngle, float, maxGradientPosY+offsetY, maxGradientPosX+offsetX) - 0.5f);
          const int binIndex = static_cast<int> ((angle >= 180.0f ? (angle-180.0f) : angle)/divisor);

          *dataIter |= 1 << binIndex;
        }

        CV_IMAGE_ELEM (sobelMagnitude, float, maxGradientPosY+offsetY, maxGradientPosX+offsetX) = -1;

        ++counter;
      }

      ++dataIter;
      offsetX += samplingSize;
    }

    offsetY += samplingSize;
    offsetX = 1;
  }

  cvReleaseImage(&sobelDx);
  cvReleaseImage(&sobelDy);
  cvReleaseImage(&sobelMagnitude);
  cvReleaseImage(&sobelAngle);
}


// ----------------------------------------------------------------------------
void
ftd::discretizers::
GradientDiscretizer::
discretize (
  IplImage * image,
  const int regionWidth,
  const int regionHeight,
  std::vector<unsigned char> & discretizedData,
  const int numOfGradients )
{
  const int horizontalSamples = image->width/regionWidth;
  const int verticalSamples = image->height/regionHeight;

  // compute gradients
  IplImage * sobelDx = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelDy = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelMagnitude = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelAngle = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);

  cvSobel (image, sobelDx, 1, 0, 3);
  cvSobel (image, sobelDy, 0, 1, 3);

  cvCartToPolar (sobelDx, sobelDy, sobelMagnitude, sobelAngle, 1); // the "1" means that the angles are in degree


  for (std::vector<unsigned char>::iterator iter = discretizedData.begin (); iter != discretizedData.end (); ++iter)
  {
    *iter = 0;
  }


  // discretize
  int offsetX = 1;
  int offsetY = 1;

  const int numOfGradientBins = 8;
  const int samplingSize = 7;
//  const int numOfGradients = 8;
  const float minimumGradientMagnitude = 10.0f;

  const float divisor = 180.0f/(numOfGradientBins-1.0f);

  std::vector<unsigned char>::iterator dataIter = discretizedData.begin ();
  for (int rowIndex = 0; rowIndex < verticalSamples; ++rowIndex)
  {
    for (int colIndex = 0; colIndex < horizontalSamples; ++colIndex)
    {
      int counter = 0;

      while (counter < numOfGradients)
      {
        float maxGradient;
        int maxGradientPosX;
        int maxGradientPosY;

        // find next location and value of maximum gradient magnitude in current region
        {
          double minValue;
          double maxValue;
          CvPoint minLocation;
          CvPoint maxLocation;

          cvSetImageROI (sobelMagnitude, cvRect (offsetX, offsetY, samplingSize, samplingSize));
          cvMinMaxLoc (sobelMagnitude, &minValue, &maxValue, &minLocation, &maxLocation);
          maxGradient = static_cast<float> (maxValue);
          maxGradientPosX = maxLocation.x;
          maxGradientPosY = maxLocation.y;
          cvResetImageROI (sobelMagnitude);
        }

        if (maxGradient < minimumGradientMagnitude)
        {
          *dataIter |= 1 << (numOfGradientBins-1);
          break;
        }
        else
        {
          const int angle = static_cast<int> (CV_IMAGE_ELEM (sobelAngle, float, maxGradientPosY+offsetY, maxGradientPosX+offsetX) - 0.5f);
          const int binIndex = static_cast<int> ((angle >= 180.0f ? (angle-180.0f) : angle)/divisor);

          *dataIter |= 1 << binIndex;
        }

        CV_IMAGE_ELEM (sobelMagnitude, float, maxGradientPosY+offsetY, maxGradientPosX+offsetX) = -1;

        ++counter;
      }

      ++dataIter;
      offsetX += samplingSize;
    }

    offsetY += samplingSize;
    offsetX = 1;
  }

  cvReleaseImage(&sobelDx);
  cvReleaseImage(&sobelDy);
  cvReleaseImage(&sobelMagnitude);
  cvReleaseImage(&sobelAngle);
}



// ----------------------------------------------------------------------------
void
ftd::discretizers::
GradientDiscretizer::
discretize (
  IplImage * image,
  const int regionWidth,
  const int regionHeight,
  std::vector<unsigned char> & discretizedData,
  std::vector<float> & strength,
  const int numOfGradients )
{
  const int horizontalSamples = image->width/regionWidth;
  const int verticalSamples = image->height/regionHeight;

  // compute gradients
  IplImage * sobelDx = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelDy = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelMagnitude = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelAngle = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);

  cvSobel (image, sobelDx, 1, 0, 3);
  cvSobel (image, sobelDy, 0, 1, 3);

  cvCartToPolar (sobelDx, sobelDy, sobelMagnitude, sobelAngle, 1); // the "1" means that the angles are in degree


  for (std::vector<unsigned char>::iterator iter = discretizedData.begin (); iter != discretizedData.end (); ++iter)
  {
    *iter = 0;
  }
  for (std::vector<float>::iterator iter = strength.begin (); iter != strength.end (); ++iter)
  {
    *iter = 0.0f;
  }


  // discretize
  const int SAMPLING_SIZE = regionWidth;
  int offsetX = 1+SAMPLING_SIZE/2;
  int offsetY = 1+SAMPLING_SIZE/2;

  const int numOfGradientBins = 8;
  const int samplingSize = 7;
  const float minimumGradientMagnitude = 10.0f;

  const float divisor = 180.0f/(numOfGradientBins-1.0f);

  float globalMaxGradient = 0.0f;
  float localMaxGradient = 0.0f;

  float gradientMagnitudeThreshold = 0.9f;


  std::vector<unsigned char>::iterator peakPointer = discretizedData.begin ();
  std::vector<float>::iterator stringPointer = strength.begin ();

  int tmpCounter = 0;
  for (int rowBinIndex = 0; rowBinIndex < verticalSamples; ++rowBinIndex)
  {
    for (int colBinIndex = 0; colBinIndex < horizontalSamples; ++colBinIndex)
    {
      std::vector<int> xCoordinates;
      std::vector<int> yCoordinates;
      std::vector<float> values;

      for (int rowPixelIndex = -SAMPLING_SIZE/2; rowPixelIndex <= SAMPLING_SIZE/2; rowPixelIndex += SAMPLING_SIZE/2)
      {
        const int yPosition = offsetY + rowPixelIndex;

        for (int colPixelIndex = -SAMPLING_SIZE/2; colPixelIndex <= SAMPLING_SIZE/2; colPixelIndex += SAMPLING_SIZE/2)
        {
          const int xPosition = offsetX + colPixelIndex;
          int counter = 0;

          // find maximum gradient magnitude in current bin
          {
            double minValue;
            double maxValue;

            cvSetImageROI (sobelMagnitude, cvRect (xPosition, yPosition, SAMPLING_SIZE, SAMPLING_SIZE));
            cvMinMaxLoc (sobelMagnitude, &minValue, &maxValue);
            localMaxGradient = static_cast<float> (maxValue);
            cvResetImageROI (sobelMagnitude);
          }

          *stringPointer += localMaxGradient;

          if (localMaxGradient > globalMaxGradient)
          {
            globalMaxGradient = localMaxGradient;
          }

          // iteratively search for the largest gradients, set it to -1, search the next largest ... etc.
          while (true)
          {
            float maxGradient;
            int maxGradientPosX;
            int maxGradientPosY;

            // find next location and value of maximum gradient magnitude in current region
            {
              double minValue;
              double maxValue;
              CvPoint minLocation;
              CvPoint maxLocation;

              cvSetImageROI (sobelMagnitude, cvRect (xPosition, yPosition, SAMPLING_SIZE, SAMPLING_SIZE));
              cvMinMaxLoc (sobelMagnitude, &minValue, &maxValue, &minLocation, &maxLocation);
              maxGradient = static_cast<float> (maxValue);
              maxGradientPosX = maxLocation.x;
              maxGradientPosY = maxLocation.y;
              cvResetImageROI (sobelMagnitude);
            }

            // TODO: really localMaxGradient and not maxGradient???
            if (localMaxGradient < minimumGradientMagnitude)
            {
              //*peakPointer |= 1 << (numOfGradientBins-1);
              break;
            }

            if ( maxGradient < localMaxGradient*gradientMagnitudeThreshold
              || counter >= numOfGradients )
            {
              break;
            }

            ++counter;

            const int angle = static_cast<int> (CV_IMAGE_ELEM (sobelAngle, float, maxGradientPosY+yPosition, maxGradientPosX+xPosition)-0.5f);
            const int binIndex = static_cast<int> ((angle >= 180 ? angle-180.0f : angle)/divisor);

            *peakPointer |= 1 << binIndex;

            xCoordinates.push_back (maxGradientPosX+xPosition);
            yCoordinates.push_back (maxGradientPosY+yPosition);
            values.push_back (maxGradient);

            CV_IMAGE_ELEM (sobelMagnitude, float, maxGradientPosY+yPosition, maxGradientPosX+xPosition) = -1.0f;
          }

          // reset values which have been set to -1
          for (unsigned int valueIndex = 0; valueIndex < values.size(); ++valueIndex)
          {
            CV_IMAGE_ELEM (sobelMagnitude, float, yCoordinates[valueIndex], xCoordinates[valueIndex]) = values[valueIndex];
          }

          xCoordinates.clear();
          yCoordinates.clear();
          values.clear();
        }
      }

      if (*peakPointer != 0)
      {
        ++tmpCounter;
      }

      ++stringPointer;
      ++peakPointer;

      offsetX += SAMPLING_SIZE;
    }

    offsetY += SAMPLING_SIZE;
    offsetX = SAMPLING_SIZE/2+1;
  }






  cvReleaseImage(&sobelDx);
  cvReleaseImage(&sobelDy);
  cvReleaseImage(&sobelMagnitude);
  cvReleaseImage(&sobelAngle);
}


// ----------------------------------------------------------------------------
void
ftd::discretizers::
GradientDiscretizer::
discretize (
  IplImage * image,
  IplImage * mask,
  const int regionWidth,
  const int regionHeight,
  std::vector<unsigned char> & discretizedData,
  const int numOfGradients )
{
  const int horizontalSamples = image->width/regionWidth;
  const int verticalSamples = image->height/regionHeight;

  // compute gradients
  IplImage * sobelDx = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelDy = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelNonMaskedMagnitude = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelMagnitude = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelAngle = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);

  cvSobel (image, sobelDx, 1, 0, 3);
  cvSobel (image, sobelDy, 0, 1, 3);

  cvCartToPolar (sobelDx, sobelDy, sobelNonMaskedMagnitude, sobelAngle, 1); // the "1" means that the angles are in degree

  cvSetZero (sobelMagnitude);
  cvCopy (sobelNonMaskedMagnitude, sobelMagnitude, mask);

  //cvShowImage ("sobelMagnitude2", sobelMagnitude);


  for (std::vector<unsigned char>::iterator iter = discretizedData.begin (); iter != discretizedData.end (); ++iter)
  {
    *iter = 0;
  }


  // discretize
  int offsetX = 1;
  int offsetY = 1;

  const int numOfGradientBins = 8;
  const int samplingSize = 7;
//  const int numOfGradients = 8;
  const float minimumGradientMagnitude = 10.0f;

  const float divisor = 180.0f/(numOfGradientBins-1.0f);

//  std::vector<unsigned char>::iterator dataIter = discretizedData.begin ();
  for (int rowIndex = 0; rowIndex < verticalSamples; ++rowIndex)
  {
    offsetY = rowIndex*samplingSize;
    offsetX = 1;

    std::vector<unsigned char>::iterator dataIter = discretizedData.begin () + rowIndex*horizontalSamples;
    for (int colIndex = 0; colIndex < horizontalSamples; ++colIndex)
    {
      int counter = 0;

      while (counter < numOfGradients)
      {
        float maxGradient;
        int maxGradientPosX;
        int maxGradientPosY;

        // find next location and value of maximum gradient magnitude in current region
        {
          double minValue;
          double maxValue;
          CvPoint minLocation;
          CvPoint maxLocation;

          cvSetImageROI (sobelMagnitude, cvRect (offsetX, offsetY, samplingSize, samplingSize));
          cvMinMaxLoc (sobelMagnitude, &minValue, &maxValue, &minLocation, &maxLocation);
          maxGradient = static_cast<float> (maxValue);
          maxGradientPosX = maxLocation.x;
          maxGradientPosY = maxLocation.y;
          cvResetImageROI (sobelMagnitude);
        }

        if (maxGradient < minimumGradientMagnitude)
        {
          //*dataIter |= 1 << (numOfGradientBins-1);
          break;
        }
        else
        {
          const int angle = static_cast<int> (CV_IMAGE_ELEM (sobelAngle, float, maxGradientPosY+offsetY, maxGradientPosX+offsetX) - 0.5f);
          const int binIndex = static_cast<int> ((angle >= 180.0f ? (angle-180.0f) : angle)/divisor);

          *dataIter |= 1 << binIndex;
        }

        CV_IMAGE_ELEM (sobelMagnitude, float, maxGradientPosY+offsetY, maxGradientPosX+offsetX) = -1;

        ++counter;
      }

      ++dataIter;
      offsetX += samplingSize;
    }

    //offsetY += samplingSize;
    //offsetX = 1;
  }

  cvReleaseImage(&sobelDx);
  cvReleaseImage(&sobelDy);
  cvReleaseImage(&sobelMagnitude);
  cvReleaseImage(&sobelAngle);
}


// ----------------------------------------------------------------------------
void
ftd::discretizers::
GradientDiscretizer::
discretize (
  IplImage * image,
  IplImage * mask,
  const int regionWidth,
  const int regionHeight,
  std::vector<unsigned char> & discretizedData,
  std::vector<float> & strength,
  const int numOfGradients )
{
  const int horizontalSamples = image->width/regionWidth;
  const int verticalSamples = image->height/regionHeight;

  // compute gradients
  IplImage * sobelDx = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelDy = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelNonMaskedMagnitude = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelMagnitude = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);
  IplImage * sobelAngle = cvCreateImage (cvGetSize (image), IPL_DEPTH_32F, 1);

  cvSobel (image, sobelDx, 1, 0, 3);
  cvSobel (image, sobelDy, 0, 1, 3);

  cvCartToPolar (sobelDx, sobelDy, sobelNonMaskedMagnitude, sobelAngle, 1); // the "1" means that the angles are in degree

  cvSetZero (sobelMagnitude);
  cvCopy (sobelNonMaskedMagnitude, sobelMagnitude, mask);
  //cvCopy (sobelNonMaskedMagnitude, sobelMagnitude);

  cvReleaseImage (&sobelNonMaskedMagnitude);

  //cvShowImage ("sobelMagnitude2", sobelMagnitude);


  for (std::vector<unsigned char>::iterator iter = discretizedData.begin (); iter != discretizedData.end (); ++iter)
  {
    *iter = 0;
  }
  for (std::vector<float>::iterator iter = strength.begin (); iter != strength.end (); ++iter)
  {
    *iter = 0.0f;
  }


  // discretize
  const int SAMPLING_SIZE = regionWidth;
  int offsetX = 1+SAMPLING_SIZE/2;
  int offsetY = 1+SAMPLING_SIZE/2;

  const int numOfGradientBins = 8;
  const int samplingSize = 7;
//  const int numOfGradients = 8;
  const float minimumGradientMagnitude = 10.0f;

  const float divisor = 180.0f/(numOfGradientBins-1.0f);

  float globalMaxGradient = 0.0f;
  float localMaxGradient = 0.0f;

  float gradientMagnitudeThreshold = 0.9f;

  //std::vector<float> stringTmp (discretizedData.size (), 0);

  std::vector<unsigned char>::iterator peakPointer = discretizedData.begin ();
  std::vector<float>::iterator stringPointer = strength.begin ();

  /*int tmpCounter = 0;
  for (int rowBinIndex = 0; rowBinIndex < verticalSamples; ++rowBinIndex)
  {
    for (int colBinIndex = 0; colBinIndex < horizontalSamples; ++colBinIndex)
    {
      std::vector<int> xCoordinates;
      std::vector<int> yCoordinates;
      std::vector<float> values;

      for (int rowPixelIndex = -SAMPLING_SIZE/2; rowPixelIndex <= SAMPLING_SIZE/2; rowPixelIndex += SAMPLING_SIZE/2)
      {
        const int yPosition = offsetY + rowPixelIndex;

        for (int colPixelIndex = -SAMPLING_SIZE/2; colPixelIndex <= SAMPLING_SIZE/2; colPixelIndex += SAMPLING_SIZE/2)
        {
          const int xPosition = offsetX + colPixelIndex;
          int counter = 0;

          // find maximum gradient magnitude in current bin
          {
            double minValue;
            double maxValue;

            cvSetImageROI (sobelMagnitude, cvRect (xPosition, yPosition, SAMPLING_SIZE, SAMPLING_SIZE));
            cvMinMaxLoc (sobelMagnitude, &minValue, &maxValue);
            localMaxGradient = static_cast<float> (maxValue);
            cvResetImageROI (sobelMagnitude);
          }

          *stringPointer += localMaxGradient;

          if (localMaxGradient > globalMaxGradient)
          {
            globalMaxGradient = localMaxGradient;
          }

          // iteratively search for the largest gradients, set it to -1, search the next largest ... etc.
          while (true)
          {
            float maxGradient;
            int maxGradientPosX;
            int maxGradientPosY;

            // find next location and value of maximum gradient magnitude in current region
            {
              double minValue;
              double maxValue;
              CvPoint minLocation;
              CvPoint maxLocation;

              cvSetImageROI (sobelMagnitude, cvRect (xPosition, yPosition, SAMPLING_SIZE, SAMPLING_SIZE));
              cvMinMaxLoc (sobelMagnitude, &minValue, &maxValue, &minLocation, &maxLocation);
              maxGradient = static_cast<float> (maxValue);
              maxGradientPosX = maxLocation.x;
              maxGradientPosY = maxLocation.y;
              cvResetImageROI (sobelMagnitude);
            }

            // TODO: really localMaxGradient and not maxGradient???
            if (localMaxGradient < minimumGradientMagnitude)
            {
              //*peakPointer |= 1 << (numOfGradientBins-1);
              break;
            }

            if ( maxGradient < localMaxGradient*gradientMagnitudeThreshold
              || counter >= numOfGradients )
            {
              break;
            }

            ++counter;

            const int angle = static_cast<int> (CV_IMAGE_ELEM (sobelAngle, float, maxGradientPosY+yPosition, maxGradientPosX+xPosition)-0.5f);
            const int binIndex = static_cast<int> ((angle >= 180 ? angle-180.0f : angle)/divisor);

            *peakPointer |= 1 << binIndex;

            xCoordinates.push_back (maxGradientPosX+xPosition);
            yCoordinates.push_back (maxGradientPosY+yPosition);
            values.push_back (maxGradient);

            CV_IMAGE_ELEM (sobelMagnitude, float, maxGradientPosY+yPosition, maxGradientPosX+xPosition) = -1.0f;
          }

          // reset values which have been set to -1
          for (unsigned int valueIndex = 0; valueIndex < values.size(); ++valueIndex)
          {
            CV_IMAGE_ELEM (sobelMagnitude, float, yCoordinates[valueIndex], xCoordinates[valueIndex]) = values[valueIndex];
          }

          xCoordinates.clear();
          yCoordinates.clear();
          values.clear();
        }
      }

      if (*peakPointer != 0)
      {
        ++tmpCounter;
      }

      ++stringPointer;
      ++peakPointer;

      offsetX += SAMPLING_SIZE;
    }

    offsetY += SAMPLING_SIZE;
    offsetX = SAMPLING_SIZE/2+1;
  }*/

  //std::cerr << "tmpCounter: " << tmpCounter << std::endl;
  //std::cerr << "numOfDiscretizedGradients: " << numOfDiscretizedGradients << std::endl;
  //std::cerr << "((horizontalSamples*verticalSamples)-numOfDiscretizedGradients): " << ((horizontalSamples*verticalSamples)-numOfDiscretizedGradients) << std::endl;

  // set smallest gradients of the template to zero such that we only
  // have NUM_OF_USED_SAMPLES bins with non-zero information
  /*for (int binCounter = 0; binCounter < ((horizontalSamples*verticalSamples)-numOfDiscretizedGradients); ++binCounter)
  {
    int minIndex = 0;
    float minValue = 10e100;
    for (int binIndex = 0; binIndex < (horizontalSamples*verticalSamples); ++binIndex)
    {
      if (stringTmp[binIndex] < minValue)
      {
        minValue = stringTmp[binIndex];
        minIndex = binIndex;
      }
    }

    stringTmp[minIndex] = 10e100;
    discretizedData[minIndex] = 0;
  }*/

  std::vector<unsigned char>::iterator dataIter = discretizedData.begin ();
  std::vector<float>::iterator strengthIter = strength.begin ();
  for (int rowIndex = 0; rowIndex < verticalSamples; ++rowIndex)
  {
    for (int colIndex = 0; colIndex < horizontalSamples; ++colIndex)
    {
      int counter = 0;

      while (counter < numOfGradients)
      {
        float maxGradient;
        int maxGradientPosX;
        int maxGradientPosY;

        // find next location and value of maximum gradient magnitude in current region
        {
          double minValue;
          double maxValue;
          CvPoint minLocation;
          CvPoint maxLocation;

          cvSetImageROI (sobelMagnitude, cvRect (offsetX, offsetY, samplingSize, samplingSize));
          cvMinMaxLoc (sobelMagnitude, &minValue, &maxValue, &minLocation, &maxLocation);
          maxGradient = static_cast<float> (maxValue);
          maxGradientPosX = maxLocation.x;
          maxGradientPosY = maxLocation.y;
          cvResetImageROI (sobelMagnitude);
        }

        if (maxGradient < minimumGradientMagnitude)
        {
          //dataIter |= 1 << (numOfGradientBins-1);
          break;
        }
        else
        {
          const int angle = static_cast<int> (CV_IMAGE_ELEM (sobelAngle, float, maxGradientPosY+offsetY, maxGradientPosX+offsetX) - 0.5f);
          const int binIndex = static_cast<int> ((angle >= 180.0f ? (angle-180.0f) : angle)/divisor);

          *dataIter |= 1 << binIndex;
          *strengthIter = std::max (*strengthIter, maxGradient);
        }

        CV_IMAGE_ELEM (sobelMagnitude, float, maxGradientPosY+offsetY, maxGradientPosX+offsetX) = -1;

        ++counter;
      }

      ++dataIter;
      ++strengthIter;
      offsetX += samplingSize;
    }

    offsetY += samplingSize;
    offsetX = 1;
  }




  cvReleaseImage(&sobelDx);
  cvReleaseImage(&sobelDy);
  cvReleaseImage(&sobelMagnitude);
  cvReleaseImage(&sobelAngle);
}




