#include <iostream>
#include "eigen-3.4.0\Eigen\Dense"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <random>
#include <iterator>
#include <algorithm>

using namespace std;
using namespace Eigen;

MatrixXd addrow(){

}

MatrixXd openData(string fileToOpen)
{
 
    // the inspiration for creating this function was drawn from here (I did NOT copy and paste the code)
    // https://stackoverflow.com/questions/34247057/how-to-read-csv-file-and-assign-to-eigen-matrix
     
    // the input is the file: "fileToOpen.csv":
    // a,b,c
    // d,e,f
    // This function converts input file data into the Eigen matrix format
 
    // the matrix entries are stored in this variable row-wise. For example if we have the matrix:
    // M=[a b c 
    //    d e f]
    // the entries are stored as matrixEntries=[a,b,c,d,e,f], that is the variable "matrixEntries" is a row vector
    // later on, this vector is mapped into the Eigen matrix format
    vector<double> matrixEntries;
 
    // in this object we store the data from the matrix
    ifstream matrixDataFile(fileToOpen);
 
    // this variable is used to store the row of the matrix that contains commas 
    string matrixRowString;
 
    // this variable is used to store the matrix entry;
    string matrixEntry;
 
    // this variable is used to track the number of rows
    int matrixRowNumber = 0;
    
    getline(matrixDataFile, matrixRowString);
    matrixRowString = "";
 
    while (getline(matrixDataFile, matrixRowString)) // here we read a row by row of matrixDataFile and store every line into the string variable matrixRowString
    {
        stringstream matrixRowStringStream(matrixRowString); //convert matrixRowString that is a string to a stream variable.
        
        while (getline(matrixRowStringStream, matrixEntry, ',')) // here we read pieces of the stream matrixRowStringStream until every comma, and store the resulting character into the matrixEntry
        {
            // matrixEntries.push_back(matrixEntries.cast<double>(matrixEntry));   //here we convert the string to double and fill in the row vector storing all the matrix entries
            matrixEntries.push_back(stod(matrixEntry));
        }
        matrixRowNumber++; //update the column numbers
    }
 
    // here we convert the vector variable into the matrix and return the resulting object, 
    // note that matrixEntries.data() is the pointer to the first memory location at which the entries of the vector matrixEntries are stored;
    return Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(matrixEntries.data(), matrixRowNumber, matrixEntries.size() / matrixRowNumber);
 
}

MatrixXd randomsubset(MatrixXd A, int n){
    
    MatrixXd subset, randomized;

    // PermutationMatrix<Dynamic,Dynamic> perm(A.rows());
    // perm.setIdentity();
    // std::random_shuffle(perm.indices().data(), perm.indices().data()+perm.indices().size());
    // randomized = perm * A;
    // subset = randomized.topRows(n);

    // std::sample(A.begin(),A.end(), subset.begin(), n, std::mt19937{std::random_device{}()});

    return Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(subset.data(), n, 3);
}

int main()
{
    int iter = 0, k = 10, n=4;
    // bestfit = null;
    // bestErr = something really big;

    // while(iter < k){
        MatrixXd A, B;
        A = openData("abc.csv");
        B = randomsubset(A, n);

        cout<<B;
    //Step 1. Select random subset of the dataset - https://stackoverflow.com/questions/9650991/pick-a-unique-random-subset-from-a-set-of-unique-values
    // (or) https://en.cppreference.com/w/cpp/algorithm/sample (or) https://www.meetingcpp.com/blog/items/randomizing-a-csv-file-with-standard-c.html
    // (or) https://www.tutorialspoint.com/cplusplus-program-to-generate-a-random-subset-by-coin-flipping
    // so far best option https://stackoverflow.com/questions/15858569/randomly-permute-rows-columns-of-a-matrix-with-eigen
    //add random index row to matrix and try - https://stackoverflow.com/questions/27404811/append-column-to-matrix-using-eigen-library
    
    
    //Step 2. Solve for the parameters of the model - 

    //Step 3. Determine the cost of fitting

    //Step 4. Add points that were not selected into the model and see how they fit (If they fit well with small error add to alsoInliers)

    //Step 5. If we get a good amount of points added to alsoInliers, compare current model to previous/best model

    //Step 6. Repeat the above steps for a certain number of iterations

    // }
}