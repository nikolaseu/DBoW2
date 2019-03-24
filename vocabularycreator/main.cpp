#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QElapsedTimer>
#include <QFileInfo>

#include <iostream>
#include <vector>

// DBoW2
#include "DBoW2/DBoW2.h" // defines OrbVocabulary and OrbDatabase

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>


void loadFeatures(const QStringList &sourceFiles, std::vector<std::vector<cv::Mat > > &features);
void changeStructure(const cv::Mat &plain, std::vector<cv::Mat> &out);


int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("DBoW2 ORB vocabulary creator");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("DBoW2 ORB vocabulary creation utility");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::OptionsAfterPositionalArgumentsMode::ParseAsOptions);
    parser.setSingleDashWordOptionMode(QCommandLineParser::SingleDashWordOptionMode::ParseAsLongOptions);

    QCommandLineOption sourceFilesRegExpOption("sourceFiles", QCoreApplication::translate("main", "Images to be used for DBoW2 ORB vocabulary creation, specified as a regular expression"),
                                               "[sourceFilesRegExp]");
    parser.addOption(sourceFilesRegExpOption);

    QCommandLineOption vocabularyFilenameOption("vocabularyFilename", QCoreApplication::translate("main", "Output vocabulary filename"),
                                               "vocabularyFilename");
    parser.addOption(vocabularyFilenameOption);

    /// Process the actual command line arguments given by the user
    parser.process(app);

    if (!parser.isSet(vocabularyFilenameOption)) {
        std::cerr << "destination directory was not specified"
                  << std::endl;
        parser.showHelp(EXIT_FAILURE);
    }

    if (!parser.isSet(sourceFilesRegExpOption)) {
        std::cerr << "source file/s were not specified"
                  << std::endl;
        parser.showHelp(EXIT_FAILURE);
    }

    const QString sourceFilesRegExp = parser.value(sourceFilesRegExpOption);
    const QFileInfo sourceFilesRegExpFileInfo(sourceFilesRegExp);
    QDir sourceFilesDir = sourceFilesRegExpFileInfo.dir();
    if (!sourceFilesDir.exists()) {\
        std::cout << "using current dir for source files regular expression"
                  << std::endl;
        sourceFilesDir = QDir::current();
    }

    const QString nameFilter = sourceFilesRegExpFileInfo.fileName();
    std::cout << "source files dir: " << sourceFilesDir.path().toStdString()
              << std::endl
              << "source files name filter: " << nameFilter.toStdString()
              << std::endl;

    QStringList sourceFiles;
    for (const auto &fileName : sourceFilesDir.entryList(QStringList() << nameFilter, QDir::Files)) {
        sourceFiles << sourceFilesDir.absoluteFilePath(fileName);
    }

    const QString vocabularyFilenameString = parser.value(vocabularyFilenameOption);

    std::vector<std::vector<cv::Mat > > features;
    loadFeatures(sourceFiles, features);

    QElapsedTimer timer;
    timer.start();

    /// branching factor and depth levels
    const int k = 10;
    const int L = 6;
    const DBoW2::WeightingType weight = DBoW2::TF_IDF;
    const DBoW2::ScoringType score = DBoW2::L1_NORM;

    OrbVocabulary voc(k, L, weight, score);

    std::cout << "Creating a " << k << "^" << L << " vocabulary ..."
              << std::endl;
    voc.create(features);
    std::cout << "... done in " << timer.elapsed() << " ms"
              << std::endl;

    std::cout << "Vocabulary information: " << std::endl
              << voc << std::endl
              << std::endl;

    timer.restart();

    /// save the vocabulary to disk
    std::cout << "Saving vocabulary to " << vocabularyFilenameString.toStdString() << " ..."
              << std::endl;
    voc.save(vocabularyFilenameString.toStdString());
    std::cout << "... done in " << timer.elapsed() << " ms"
              << std::endl;

    timer.restart();

    /// load the vocabulary from disk
    std::cout << "Test reading vocabulary from " << vocabularyFilenameString.toStdString() << " ..."
              << std::endl;
    OrbVocabulary vocFromFile(vocabularyFilenameString.toStdString());
    std::cout << "... done in " << timer.elapsed() << " ms"
              << std::endl;

    std::cout << "Loaded vocabulary information: " << std::endl
              << vocFromFile << std::endl
              << std::endl;

    return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------

void loadFeatures(const QStringList &sourceFiles, std::vector<std::vector<cv::Mat > > &features)
{
    QElapsedTimer timer;
    timer.start();

    features.clear();
    features.reserve(sourceFiles.size());

    cv::Ptr<cv::ORB> orb = cv::ORB::create();

    std::cout << "Extracting ORB features..." << std::endl;
    for (const auto &file : sourceFiles) {
        std::cout << "loading " << file.toStdString()
                  << std::endl;

        cv::Mat image = cv::imread(file.toStdString(), cv::IMREAD_GRAYSCALE);
        cv::Mat mask;
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;

        orb->detectAndCompute(image, mask, keypoints, descriptors);

        features.push_back(std::vector<cv::Mat >());
        changeStructure(descriptors, features.back());
    }

    std::cout << "extracted ORB features in " << timer.elapsed() << " ms. Approx time per image: " << float(timer.elapsed()) / sourceFiles.size() << " ms/img"
              << std::endl;
}

// ----------------------------------------------------------------------------

void changeStructure(const cv::Mat &plain, std::vector<cv::Mat> &out)
{
    out.resize(plain.rows);

    for(int i = 0; i < plain.rows; ++i) {
        out[i] = plain.row(i);
    }
}
