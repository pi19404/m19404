
//#include <QtGui/QApplication>
//#include "mainwindow.h"


#include <mlpack/core.hpp>
#include <mlpack/methods/kmeans/kmeans.hpp>
#include <mlpack/methods/gmm/gmm.hpp>
#include <mlpack/methods/gmm/em_fit.hpp>
#include <mlpack/methods/gmm/em_fit_impl.hpp>
// Document program.
PROGRAM_INFO("K-Means Clustering", "This program performs K-Means clustering "
    "on the given dataset, storing the learned cluster assignments either as "
    "a column of labels in the file containing the input dataset or in a "
    "separate file. ");

PARAM_STRING_REQ("inputFile", "Input dataset to perform clustering on.", "i");
PARAM_INT_REQ("clusters", "Number of clusters to find.", "c");
PARAM_STRING("centroid_file", "If specified, the centroids of each cluster will"
    " be written to the given file.", "C", "");
PARAM_INT("max_iterations", "Maximum number of iterations before K-Means "
    "terminates.", "m", 1000);
PARAM_INT("seed", "Random seed.  If 0, 'std::time(NULL)' is used.", "s", 0);


using namespace mlpack;
using namespace mlpack::distribution;
using namespace mlpack::util;
using namespace mlpack::gmm;
using namespace mlpack::math;
using namespace mlpack::gmm;
using namespace arma;
using namespace std;

using namespace mlpack;
using namespace std;
using namespace mlpack;
using namespace mlpack::kmeans;
using namespace mlpack::util;


int main(int argc, char *argv[])
{

  CLI::ParseCommandLine(argc, argv);

  // Initialize random seed.
  if (CLI::GetParam<int>("seed") != 0)
    math::RandomSeed((size_t) CLI::GetParam<int>("seed"));
  else
    math::RandomSeed((size_t) std::time(NULL));

  // Now do validation of options.
  string inputFile = CLI::GetParam<string>("inputFile");
  string cfile=CLI::GetParam<string>("centroid_file");
  int clusters = CLI::GetParam<int>("clusters");
  if (clusters < 1)
  {
    Log::Fatal << "Invalid number of clusters requested (" << clusters << ")! "
        << "Must be greater than or equal to 1." << std::endl;
  }

  int maxIterations = CLI::GetParam<int>("max_iterations");
  if (maxIterations < 0)
  {
    Log::Fatal << "Invalid value for maximum iterations (" << maxIterations <<
        ")! Must be greater than or equal to 0." << std::endl;
  }


  // Load our dataset.
  arma::mat dataset;
  data::Load(inputFile.c_str(), dataset, true); // Fatal upon failure.

  // Now create the KMeans object.  Because we could be using different types,
  // it gets a little weird...
  arma::Col<size_t> assignments;
  arma::mat centroids;

  KMeans<> k(maxIterations,1.0);


  Log::Debug << "Performing K Means Clustering " << endl;
  k.Cluster(dataset, clusters, assignments, centroids);
  Log::Debug << "Completed Performing K Means Clustering " << endl;




//  for (size_t i = 0; i < clusters; ++i)
  //{
  //    std::stringstream o;
    //  o << i;
    //  std::string meanName = "centroid" + o.str();

      //save.SaveParameter(centroids.cols(i), meanName);
  //}

  /*save.SaveParameter(weights, "weights");
  for (size_t i = 0; i < gaussians; ++i)
  {
    // Generate names for the XML nodes.
    std::stringstream o;
    o << i;
    std::string meanName = "mean" + o.str();
    std::string covName = "covariance" + o.str();

    // Now save them.
    save.SaveParameter(means[i], meanName);
    save.SaveParameter(covariances[i], covName);
  }*/

  Log::Debug << "Writing Cluster Centroids to file" << cfile << endl;



  std::vector<arma::vec> means(clusters,arma::vec(dataset.n_rows));
  std::vector<arma::mat> covariances(clusters,arma::mat(dataset.n_rows,dataset.n_rows));
   arma::vec weights(clusters);


      for (size_t i = 0; i < means.size(); ++i)
      {
          means[i].fill(0);
          covariances[i].fill(0);
      }

      Log::Debug << "completed initialization of mean and covariance matrices" << endl;

      Log::Debug << "Dataset consists of  " << dataset.n_cols << " elements of dimensionality " << dataset.n_rows << endl;
      // From the assignments, generate our means, covariances, and weights.
      for (int i = 0; i < dataset.n_cols;i++)
      {
        int clusterx = assignments[i];
        means[clusterx] += dataset.col(i);
        weights[clusterx]=weights[clusterx]+1;
      }

      arma::mat ver(centroids);
      ver.fill(0);

      for(int i=0;i<clusters;i++)
      {
          ver.col(i)=means[i]/weights[i];
      }

      metric::SquaredEuclideanDistance m;
      double err=0;
      for(int i=0;i<clusters;i++)
      {
          err= err+m.Evaluate(ver.col(i).t(),centroids.col(i).t()) ;
      }
    Log::Debug << "Verying result of KMeans Clustering " <<err<< endl;


    Log::Debug << "Completed calculation of means of dataset " << endl;


    for (size_t i = 0; i < dataset.n_cols; ++i)
    {
      const size_t cluster = assignments[i];
      const arma::vec normObs = dataset.col(i) - means[cluster];
      covariances[cluster] += normObs * normObs.t();
    }


    for (size_t i = 0; i < means.size(); ++i)
    {
      covariances[i] /= (weights[i] > 1) ? weights[i] : 1;

      // Ensure positive-definiteness.  TODO: make this more efficient.
      if (true && det(covariances[i]) <= 1e-50)
      {
        Log::Debug << "Covariance matrix " << i << " is not positive definite. "<< "Adding perturbation." << std::endl;

        double perturbation = 1e-50;
        while (det(covariances[i]) <= 1e-50)
        {
          covariances[i].diag() += perturbation;
          perturbation *= 10; // Slow, but we don't want to add too much.
        }
      }
    }

    Log::Debug << "Normalizing the covariance matrices " << endl;
    // Finally, normalize weights.
    weights /= accu(weights);

    Log::Debug << "Normalizing the weights "     << endl;

    Log::Debug << "Completed calculation of covariance of dataset " << endl;

      util::SaveRestoreUtility save;

      save.SaveParameter(centroids,"centroids");
      for(int i=0;i<clusters;i++)
      {
          std::stringstream o;
          o << i;
          std::string meanName = "covariance" + o.str();
          save.SaveParameter(covariances[i],meanName);
      }
    save.SaveParameter(clusters, "clusters");


      if (!save.WriteFile(cfile))
          Log::Warn << "Error writing to file " << endl;




      double logLikelihood = 0;

       arma::vec phis;
       arma::mat likelihoods(means.size(), dataset.n_cols);
       for (size_t i = 0; i < means.size(); ++i)
       {
         phi(dataset, means[i], covariances[i], phis);
         likelihoods.row(i) = weights(i) * trans(phis);
       }

       // Now sum over every point.
       for (size_t j = 0; j < dataset.n_cols; ++j)
       {
         if (accu(likelihoods.col(j)) == 0)
           Log::Info << "Likelihood of point " << j << " is 0!  It is probably an "<< "outlier." << std::endl;
         else
         logLikelihood += log(accu(likelihoods.col(j)));
       }


   Log::Debug << "EMFit::Estimate(): initial clustering log-likelihood: "<< logLikelihood << std::endl;

  Log::Info << "Completed Program" << std::endl;


}

/*#include <mlpack/core.hpp>
#include <mlpack/core.hpp>
#include "mlpack/methods/kmeans/kmeans.hpp"
#include "mlpack/methods/kmeans/allow_empty_clusters.hpp"
#include "mlpack/methods/kmeans/refined_start.hpp"

using namespace mlpack;
using namespace mlpack::kmeans;
using namespace std;
using namespace mlpack::util;

// Define parameters for the executable.
PROGRAM_INFO("K-Means Clustering", "This program performs K-Means clustering "
    "on the given dataset, storing the learned cluster assignments either as "
    "a column of labels in the file containing the input dataset or in a "
    "separate file.  Empty clusters are not allowed by default; when a cluster "
    "becomes empty, the point furthest from the centroid of the cluster with "
    "maximum variance is taken to fill that cluster."
    "\n\n"
    "Optionally, the Bradley and Fayyad approach (\"Refining initial points for"
    " k-means clustering\", 1998) can be used to select initial points by "
    "specifying the --refined_start (-r) option.  This approach works by taking"
    " random samples of the dataset; to specify the number of samples, the "
    "--samples parameter is used, and to specify the percentage of the dataset "
    "to be used in each sample, the --percentage parameter is used (it should "
    "be a value between 0.0 and 1.0)."
    "\n\n"
    "If you want to specify your own initial cluster assignments or initial "
    "cluster centroids, this functionality is available in the C++ interface. "
    "Alternately, file a bug (well, a feature request) on the mlpack bug "
    "tracker.");

// Required options.
PARAM_STRING_REQ("inputFile", "Input dataset to perform clustering on.", "i");
PARAM_INT_REQ("clusters", "Number of clusters to find.", "c");

// Output options.
PARAM_FLAG("in_place", "If specified, a column of the learned cluster "
    "assignments will be added to the input dataset file.  In this case, "
    "--outputFile is not necessary.", "p");
PARAM_STRING("output_file", "File to write output labels or labeled data to.",
    "o", "output.csv");
PARAM_STRING("centroid_file", "If specified, the centroids of each cluster will"
    " be written to the given file.", "C", "");

// k-means configuration options.
PARAM_FLAG("allow_empty_clusters", "Allow empty clusters to be created.", "e");
PARAM_FLAG("labels_only", "Only output labels into output file.", "l");
PARAM_DOUBLE("overclustering", "Finds (overclustering * clusters) clusters, "
    "then merges them together until only the desired number of clusters are "
    "left.", "O", 1.0);
PARAM_INT("max_iterations", "Maximum number of iterations before K-Means "
    "terminates.", "m", 1000);
PARAM_INT("seed", "Random seed.  If 0, 'std::time(NULL)' is used.", "s", 0);

// This is known to not work (#251).
//PARAM_FLAG("fast_kmeans", "Use the experimental fast k-means algorithm by "
//    "Pelleg and Moore.", "f");

// Parameters for "refined start" k-means.
PARAM_FLAG("refined_start", "Use the refined initial point strategy by Bradley "
    "and Fayyad to choose initial points.", "r");
PARAM_INT("samplings", "Number of samplings to perform for refined start (use "
    "when --refined_start is specified).", "S", 100);
PARAM_DOUBLE("percentage", "Percentage of dataset to use for each refined start"
    " sampling (use when --refined_start is specified).", "p", 0.02);


int main(int argc, char** argv)
{
  CLI::ParseCommandLine(argc, argv);

  Log::Debug << "Compiled with debugging symbols 1" << std::endl;


  // Initialize random seed.
  if (CLI::GetParam<int>("seed") != 0)
    math::RandomSeed((size_t) CLI::GetParam<int>("seed"));
  else
    math::RandomSeed((size_t) std::time(NULL));

  // Now do validation of options.
  string inputFile = CLI::GetParam<string>("inputFile");
  int clusters = CLI::GetParam<int>("clusters");
  if (clusters < 1)
  {
    Log::Fatal << "Invalid number of clusters requested (" << clusters << ")! "
        << "Must be greater than or equal to 1." << std::endl;
  }

  int maxIterations = CLI::GetParam<int>("max_iterations");
  if (maxIterations < 0)
  {
    Log::Fatal << "Invalid value for maximum iterations (" << maxIterations <<
        ")! Must be greater than or equal to 0." << std::endl;
  }

  double overclustering = CLI::GetParam<double>("overclustering");
  if (overclustering < 1)
  {
    Log::Fatal << "Invalid value for overclustering (" << overclustering <<
        ")! Must be greater than or equal to 1." << std::endl;
  }

  // Make sure we have an output file if we're not doing the work in-place.
  if (!CLI::HasParam("in_place") && !CLI::HasParam("output_file"))
  {
    Log::Fatal << "--outputFile not specified (and --in_place not set)."
        << std::endl;
  }

  // Load our dataset.
  arma::mat dataset;
  data::Load(inputFile.c_str(), dataset, true); // Fatal upon failure.

  // Now create the KMeans object.  Because we could be using different types,
  // it gets a little weird...
  arma::Col<size_t> assignments;
  arma::mat centroids;

  if (CLI::HasParam("allow_empty_clusters"))
  {
    if (CLI::HasParam("refined_start"))
    {
      const int samplings = CLI::GetParam<int>("samplings");
      const double percentage = CLI::GetParam<int>("percentage");

      if (samplings < 0)
        Log::Fatal << "Number of samplings (" << samplings << ") must be "
            << "greater than 0!" << std::endl;
      if (percentage <= 0.0 || percentage > 1.0)
        Log::Fatal << "Percentage for sampling (" << percentage << ") must be "
            << "greater than 0.0 and less than or equal to 1.0!" << std::endl;

      KMeans<metric::SquaredEuclideanDistance, RefinedStart, AllowEmptyClusters>
          k(maxIterations, overclustering, metric::SquaredEuclideanDistance(),
          RefinedStart(samplings, percentage));

      Timer::Start("clustering");
      if (CLI::HasParam("fast_kmeans"))
        k.FastCluster(dataset, clusters, assignments);
      else
        k.Cluster(dataset, clusters, assignments, centroids);
      Timer::Stop("clustering");
    }
    else
    {
      KMeans<metric::SquaredEuclideanDistance, RandomPartition,
          AllowEmptyClusters> k(maxIterations, overclustering);

      Timer::Start("clustering");
      if (CLI::HasParam("fast_kmeans"))
        k.FastCluster(dataset, clusters, assignments);
      else
        k.Cluster(dataset, clusters, assignments, centroids);
      Timer::Stop("clustering");
    }
  }
  else
  {
    if (CLI::HasParam("refined_start"))
    {
      const int samplings = CLI::GetParam<int>("samplings");
      const double percentage = CLI::GetParam<int>("percentage");

      if (samplings < 0)
        Log::Fatal << "Number of samplings (" << samplings << ") must be "
            << "greater than 0!" << std::endl;
      if (percentage <= 0.0 || percentage > 1.0)
        Log::Fatal << "Percentage for sampling (" << percentage << ") must be "
            << "greater than 0.0 and less than or equal to 1.0!" << std::endl;

      KMeans<metric::SquaredEuclideanDistance, RefinedStart, AllowEmptyClusters>
          k(maxIterations, overclustering, metric::SquaredEuclideanDistance(),
          RefinedStart(samplings, percentage));

      Timer::Start("clustering");
      if (CLI::HasParam("fast_kmeans"))
        k.FastCluster(dataset, clusters, assignments);
      else
        k.Cluster(dataset, clusters, assignments, centroids);
      Timer::Stop("clustering");
    }
    else
    {
      KMeans<> k(maxIterations, overclustering);

      Timer::Start("clustering");
      if (CLI::HasParam("fast_kmeans"))
        k.FastCluster(dataset, clusters, assignments);
      else
        k.Cluster(dataset, clusters, assignments, centroids);
      Timer::Stop("clustering");
    }
  }

  // Now figure out what to do with our results.
  if (CLI::HasParam("in_place"))
  {
    // Add the column of assignments to the dataset; but we have to convert them
    // to type double first.
    arma::vec converted(assignments.n_elem);
    for (size_t i = 0; i < assignments.n_elem; i++)
      converted(i) = (double) assignments(i);

    dataset.insert_rows(dataset.n_rows, trans(converted));

    // Save the dataset.
    data::Save(inputFile, dataset);
  }
  else
  {
    if (CLI::HasParam("labels_only"))
    {
      // Save only the labels.
      string outputFile = CLI::GetParam<string>("output_file");
      arma::Mat<size_t> output = trans(assignments);
      data::Save(outputFile, output);
    }
    else
    {
      // Convert the assignments to doubles.
      arma::vec converted(assignments.n_elem);
      for (size_t i = 0; i < assignments.n_elem; i++)
        converted(i) = (double) assignments(i);

      dataset.insert_rows(dataset.n_rows, trans(converted));

      // Now save, in the different file.
      string outputFile = CLI::GetParam<string>("output_file");
      data::Save(outputFile, dataset);
    }
  }

  // Should we write the centroids to a file?
  if (CLI::HasParam("centroid_file"))
    data::Save(CLI::GetParam<std::string>("centroid_file"), centroids);

}
*/
