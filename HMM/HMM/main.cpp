//#include <QtGui/QApplication>
//#include "mainwindow.h"


//#include <mlpack/core.hpp>
//#include <mlpack/methods/gmm/gmm.hpp>
//#include <mlpack/core/dists/gaussian_distribution.hpp>

//// Document program.
//PROGRAM_INFO("Gaussian Mixture Models", "This program performs Custering "
//    "using Gaussian Mixture Models.Each Feature vector is assigned a probability "
//    "of belonging to one of clusters.");



//PARAM_STRING("input_file", "File containing the data on which the model will be fit.", "i","");
//PARAM_INT("gaussians", "Number of Gaussians in the GMM.", "g", 1);
//PARAM_STRING("model_file", "The file to write the trained GMM parameters into "
//        "(as XML).", "o", "gmm.xml");
//PARAM_STRING("method", "The method used to represent feature vectors"
//    "(as XML).", "m", "gmm");

//using namespace mlpack;
//using namespace std;
//using namespace mlpack;
//using namespace mlpack::gmm;
//using namespace mlpack::util;
//using namespace mlpack::distribution;

//int main(int argc, char *argv[])
//{
    
//  CLI::ParseCommandLine(argc, argv);

//  Log::Debug << "Starting With Gaussian Mixture Model Program" <<endl;
//  Log::Info << "Loading Data From CSV File" << endl;


//  GaussianDistribution gaussian;


//  GMM<> gmm(1,1);
//  arma::mat dataPoints;

//  string input_file=CLI::GetParam<std::string>("input_file");



//  string modelFile=CLI::GetParam<string>("model_file");
//  if(input_file=="")
//  {
//  SaveRestoreUtility loader;
//  loader.ReadFile(modelFile);
//  Log::Info << "Loading Data From the Model Files" << std::endl;
//  gmm.Load (modelFile.c_str ());
//  Log::Info << "Number of Gaussians " << gmm.Gaussians ()<<endl;
//  Log::Info << "Dimensionality " << gmm.Dimensionality ()<< endl;

//  Log::Info << "Completed Loading The Model file " << endl;
//  }
//  else
//  {
//      int gaussians = CLI::GetParam<int>("gaussians");
//      data::Load(input_file.c_str(), dataPoints, true);

//     EMFit<> em(250, 1e-10,true);

//      gmm=GMM<> (size_t(gaussians), dataPoints.n_rows,em);


//  Log::Info << "Setting up a mixture with " <<gaussians<< " gaussians in " << dataPoints.n_rows <<" dimensional space " <<endl;
//  Log::Info << "number of data points are " << dataPoints.n_cols << endl;
// std::vector<arma::vec> probabilities(1,arma::vec(dataPoints.n_cols));
// Timer::Start("Starting GMM Estimation");
// gmm.Estimate(dataPoints,probabilities[0],1);
//    Timer::Stop("Completed GMM Estimation");


//    ////// OUTPUT RESULTS //////
///*      SaveRestoreUtility save;
//      save.SaveParameter(gmm.Gaussians(), "gaussians");
//      save.SaveParameter(gmm.Dimensionality(), "dimensionality");
//      save.SaveParameter(trans(gmm.Weights()), "weights");
//      for (size_t i = 0; i < gmm.Gaussians(); ++i)
//      {
//        // Generate names for the XML nodes.
//        std::stringstream o;
//        o << i;
//        std::string meanName = "mean" + o.str();
//        std::string covName = "covariance" + o.str();
//        // Now save them.
//        save.SaveParameter(trans(gmm.Means()[0]), meanName);
//        save.SaveParameter(gmm.Covariances()[0], covName);
//      }

//      Log::Info << "Storing The Results" << std::endl;
//      save.WriteFile(CLI::GetParam<std::string>("output_file").c_str());*/
//    gmm.Save (modelFile.c_str ());
//  }



//  Log::Info << "Completed Program" << std::endl;


//}
