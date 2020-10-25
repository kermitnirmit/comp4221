//
// Created by Dekai WU and YAN Yuchen on 20190307.
//

#include "assignment.hpp"
#include <fstream>
using namespace std;

// TODO: put your student ID in this variable
const char* STUDENT_ID = "20622139";

// TODO: define your global variables here if you need
token_feedforward_classifier* tffc; 
/**
 * initialize your model. this function will be called before the "train" function
 * \param vocab the vocabulary, represented as a list of unique strings
 * \param labels the set of all possible labels
 */
void init(const std::vector<std::string>& vocab, const std::vector<std::string>& labels) {
  // TODO: do whatever necessary to initialize your model
  // hint: you need to choose an appropriate embedding size and number of hidden layers
  tffc = new token_feedforward_classifier(vocab, 30, 1, labels);
}

/**
 * train your model with a training set
 * \param tokens the list of all training tokens
 * \param oracles the list of the desired label for the training tokens
 */
void train(const std::vector<std::string> &tokens, const std::vector<std::string> &oracles) {
  // TODO: complete this training function
  // hint: you need to choose an appropriate number of epochs
  tffc -> train(tokens, oracles, 6);
}

/**
 * use your model to predict POS tag
 * \param tokens the list of tokens to perform POS tag
 * \return the list of predicted POS tags
 */
std::vector<std::string> test(const std::vector<std::string> &tokens) {
  // TODO: complete this testing function
  return tffc -> test(tokens);
}
