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

std::vector<int> possibilities;

void addrow(MatrixXd *mat1, VectorXd vec){

    mat1->conservativeResize(mat1->rows()+1, mat1->cols());
    mat1->row(mat1->rows()-1) = vec;

}

void addcol(MatrixXd *mat2, VectorXd vec){

    mat2->conservativeResize(mat2->rows(), mat2->cols()+1);
    mat2->col(mat2->cols()-1) = vec;

}

VectorXd ones_mat_col_gen(int n){
    VectorXd id(n,1);
    for(int i=0; i<n; i++){
        id(i) = 1;
   }
   return id;
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

double cost(MatrixXd sol, MatrixXd A){
    double x0, y0, z0, r;
    x0 = sol(0)/2;
    y0 = sol(1)/2;
    z0 = sol(2)/2;
    r = sqrt(sol(3)+pow(x0,2)+pow(y0,2)+pow(z0,2));
    RowVectorXd center(3);
    center<<x0, y0, z0;
    MatrixXd d(A.rows(),1);
    RowVectorXd rad(1);
    rad << r;
    MatrixXd del = A.rowwise() - center;
    d = del.rowwise().norm();
    MatrixXd diff1 = d.rowwise() - rad;
    MatrixXd result = diff1.colwise().squaredNorm();
    double rms = result(0);
    rms = rms/del.rows();
    rms = sqrt(rms);
    return rms;
}

MatrixXd randomsubset(MatrixXd &A, int n){
    
    MatrixXd subset(0,3), randomized;

    std::vector<int> possibilities(A.rows());
    for (int i = 0; i < A.rows(); i++)
    {
        possibilities[i] = i;
    }

    std::random_shuffle(possibilities.begin(), possibilities.end());

    for (int i = 0; i< n; i++){
        addrow(&subset, A.row(possibilities[i]));
    }

    return Map<Matrix<double, Dynamic, Dynamic, RowMajor>>(subset.data(), subset.rows(), 3);
}

int main()
{
    int iter = 0, k = 10, n=4;
    // double x0, y0, z0, r;

    // bestfit = null;
    // bestErr = something really big;

    // while(iter < k){
        MatrixXd A, B, B_aug, A_aug;

        A = openData("incorrect.csv");

        
    //Step 1. Select random subset of the dataset - https://stackoverflow.com/questions/9650991/pick-a-unique-random-subset-from-a-set-of-unique-values
    // (or) https://en.cppreference.com/w/cpp/algorithm/sample (or) https://www.meetingcpp.com/blog/items/randomizing-a-csv-file-with-standard-c.html
    // (or) https://www.tutorialspoint.com/cplusplus-program-to-generate-a-random-subset-by-coin-flipping
    // so far best option https://stackoverflow.com/questions/15858569/randomly-permute-rows-columns-of-a-matrix-with-eigen
    //add random index row to matrix and try - https://stackoverflow.com/questions/27404811/append-column-to-matrix-using-eigen-library
    B = randomsubset(A, n);
    
    //Step 2. Solve for the parameters of the model - 
    B_aug = B;
    VectorXd id1 = ones_mat_col_gen(n);
    addcol(&B_aug, id1);
    MatrixXd f1;
    f1 = B.rowwise().squaredNorm();
    MatrixXd sol1 = (((B_aug.transpose() * B_aug).inverse())*B_aug.transpose())* f1;

    //Step 3. Determine the cost of fitting
    double cost_subset = cost(sol1, B);

    //Step 4. Add points that were not selected into the model and see how they fit (If they fit well with small error add to alsoInliers)

    A_aug = A;
    VectorXd id2 = ones_mat_col_gen(A.rows());
    addcol(&A_aug, id2);
    MatrixXd f2;
    f2 = A.rowwise().squaredNorm();
    MatrixXd sol2 = (((A_aug.transpose() * A_aug).inverse())*A_aug.transpose())* f2;
    double cost_whole = cost(sol2, A);
    cout<<cost_subset<<endl<<cost_whole;

    //Step 5. If we get a good amount of points added to alsoInliers, compare current model to previous/best model


    //Step 6. Repeat the above steps for a certain number of iterations

    // }
}