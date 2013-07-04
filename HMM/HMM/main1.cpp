//#include <QtGui/QApplication>
//#include "mainwindow.h"


//#include <mlpack/core.hpp>
//#include <mlpack/methods/hmm/hmm.hpp>
//#include <mlpack/methods/hmm/hmm_util.hpp>
//#include <mlpack/methods/gmm/gmm.hpp>
//#include <mlpack/core/dists/gaussian_distribution.hpp>

//// Document program.
//PROGRAM_INFO("Hidden Mixture Models", "This program accepts sequence of continuous observation"
//             "vectors as input and trains HMM.The sequence of continous observation are stored in a CSV files and"
//             "file containing location of CSV file is provided as input to the program");

//PARAM_STRING_REQ("input_file", "File containing the path to training data files", "i");
//PARAM_INT("states", "Number of hidden states in HMM (necessary, unless "
//    "model_file is specified.", "n", 0);
//PARAM_INT("gaussians", "Number of Gaussians in the GMM.", "g", 1);
//PARAM_STRING("model_file", "The file to write the trained HMM parameters into "
//        "(as XML).", "o", "hmm.xml");
//PARAM_INT("seed", "Random seed.  If 0, 'std::time(NULL)' is used.", "s", 0);
//PARAM_DOUBLE("tolerance", "Tolerance of the Baum-Welch algorithm.", "T", 1e-5);

//using namespace mlpack;
//using namespace mlpack::hmm;
//using namespace mlpack::distribution;
//using namespace mlpack::util;
//using namespace mlpack::gmm;
//using namespace mlpack::math;
//using namespace arma;
//using namespace std;

//int main(int argc, char *argv[])
//{

//  CLI::ParseCommandLine(argc, argv);

//  Log::Debug << "Starting With Hidden Markov Model Program" <<endl;
//  Log::Warn << "Loading Data From CSV File" << endl;

//  if (CLI::GetParam<int>("seed") != 0)
//    RandomSeed((size_t) CLI::GetParam<int>("seed"));
//  else
//    RandomSeed((size_t) time(NULL));


//  const string inputFile = CLI::GetParam<string>("input_file");
//  int gaussians = CLI::GetParam<int>("gaussians");
//  string modelFile=CLI::GetParam<string>("model_file");
//  const double tolerance = CLI::GetParam<double>("tolerance");
//const int states = CLI::GetParam<int>("states");

//  vector<mat> trainSeq; //list of training matrices,each matrix for  observation sample


//  Log::Warn << "Reading list of training sequences from '" << inputFile
//      << "'." << endl;

//  fstream f(inputFile.c_str(), ios_base::in);

//  if (!f.is_open())
//    Log::Fatal << "Could not open '" << inputFile << "' for reading." << endl;

//  // Now read each line in.
//  char lineBuf[1024]; // Max 1024 characters... hopefully that is long enough.
//  f.getline(lineBuf, 1024, '\n');
//  int index=0;

//  const double noise = 0.001;

//  while (!f.eof())
//  {
//    Log::Info << "Adding training sequence from '" << lineBuf << "'." << endl;

//    // Now read the matrix.
//    trainSeq.push_back(mat());
//    data::Load(lineBuf, trainSeq.back(), true);
//    trainSeq[index] += noise * arma::randn(trainSeq[index].n_rows, trainSeq[index].n_cols);
//    index=index+1;
//     f.getline(lineBuf, 1024, '\n');
//    }
//  f.close ();
//Log::Warn << "Added zero-mean Gaussian noise with variance " << noise<< " to dataset." << std::endl;
//Log::Warn << "Stated with HMM Training for "<<index  <<" observation sequcens" << std::endl;

//  HMM<GMM<> > hmm(1, GMM<>(1, 1));
//  int dimensionality = trainSeq[0].n_rows;


//  hmm = HMM<GMM<> >(size_t(states), GMM<>(size_t(gaussians),dimensionality), tolerance);

//  Log::Warn << "Dimensionality  " << dimensionality << "  Gaussians " <<gaussians << endl;

//  for (size_t i = 0; i < trainSeq.size(); ++i)
//    if (trainSeq[i].n_rows != dimensionality)
//      Log::Fatal << "Observation sequence " << i << " dimensionality ("
//          << trainSeq[i].n_rows << " is incorrect (should be "
//          << dimensionality << ")!" << endl;

// hmm.Train(trainSeq);

// SaveRestoreUtility sr;
// SaveHMM(hmm, sr);
// sr.WriteFile(modelFile);


//  Log::Warn << "Completed Program" << std::endl;


//}

