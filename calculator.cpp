// Junji Bressan
// CPSC 301 Section 1
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

class digit
{
    public:
    int data;
    digit *next = nullptr;
};

// Suggested functions
digit * loadNumber(ifstream & file);
int digcmp(digit * left, digit *right);
char getOperator(ifstream & file);
void writeNumber(digit * num, ofstream & file);
void deleteNumber(digit * num);
digit * addNumbers(digit * left, digit * right);
digit * subNumbers(digit * left, digit * right);
void setNeg(digit * num);
void subCarry(digit * head);
digit * sub_Size_Equal_Left_Greater(digit * left, digit * right);
digit * sub_Size_Equal_Right_Greater(digit * left, digit * right);
digit * sub_Right_Length_Greater(digit * left, digit * right);
digit * sub_Left_Length_Greater(digit * left, digit *right);

int main()
{
    digit * left, * right, * result;
    char charOperator;
    int num;
    ifstream inFile ("largeNumbers.txt");
    ofstream outFile ("output.txt");

    while (!inFile.eof())
    {
        // implement program logic here
        left = loadNumber(inFile);
        right = loadNumber(inFile);
        charOperator = getOperator(inFile);

        if(charOperator == '+')
        {
          result = addNumbers(left, right);
          writeNumber(result, outFile);
          deleteNumber(result);
        }
        if(charOperator == '-')
        {
          result = subNumbers(left, right);
          writeNumber(result, outFile);
          deleteNumber(result);
        }
    }
    outFile.close();
    inFile.close();
    return 0;
}

// =============================================================================
/*
*  returns a number <0 || 0 || >0
*  <0 right number is larger
*  0  the numbers are equal
*  >0 left number is larger
*  only works if the number is stored in reverse order
*  CODED BY INSTRUCTOR
*/
int digcmp(digit * left, digit * right){
    int result = 0;
    do
    {
        if (left->data - right->data == 0)
        {
          // take no action
        }
        else
        {
          result = left->data - right->data;
        }
        left = left->next;
        right = right->next;

    } while (left != nullptr && right != nullptr);
    result = 0;
    if (left == nullptr && right != nullptr)
    {
        // right number is larger
        result = -1;
    }

    if (left != nullptr && right == nullptr)
    {
        // left number is larger
        result = 1;
    }
    return result;
}
// =============================================================================

// =============================================================================
// This function will read from largeNumbers.txt and load each individual
// digit into a linked list in reverse order. It will stop when the number on
// that particular line ends. It returns the head pointer pointing to the front
// of the list. i.e. original: 1234567  list: 7654321
digit * loadNumber(ifstream & file)
{
  digit *head = nullptr;
  digit *newDigit = nullptr;
  char charNum;
  int num;

  while(!file.eof())
  {
    file.get(charNum);
    if(charNum == '\n')
    {
      break;
    }
    newDigit = new digit;
    if(head == nullptr)
    {
      num = charNum - 48;
      newDigit->data = num;
      head = newDigit;
    }
    else
    {
      num = charNum - 48;
      newDigit->data = num;
      newDigit->next = head;
      head = newDigit;
    }
  }
  return head;
}
// =============================================================================

// =============================================================================
// This function reads from largeNumbers.txt and grabs the operator. It returns
// the operator it grabs.
char getOperator(ifstream & file)
{
  char op;
  string discard;

  file.get(op);
  getline(file, discard);

  return op;
}
// =============================================================================

// =============================================================================
// This function will write the data in the linked list to output.txt.
void writeNumber(digit * num, ofstream & file)
{
  digit *writeDigit = nullptr;

  for(writeDigit = num; writeDigit != nullptr; writeDigit = writeDigit->next)
  {
    file << writeDigit->data;
  }
  file << endl;
}
// =============================================================================

// =============================================================================
// This function will delete the data in the list once it is no longer needed.
void deleteNumber(digit * num)
{
  digit *deleteDigit = nullptr;
  while(num != nullptr)
  {
    deleteDigit = num;
    num = num->next;
    delete deleteDigit;
  }
}
// =============================================================================

// =============================================================================
// This function will take the left and right lists and add them digit by digit.
// Depending on the length of each list a result will be calculated. If the
// result is a single digit number it will be inserted to a new list pointed to by sum.
// If result is double digit then the result will be split into two digits and
// add the first digit to the list and use the second digit for the calculation
// for the next result. The sum pointer pointing to the new list will be returned
// once all calculations are completed.
digit * addNumbers(digit * left, digit * right)
{
  digit *sum = nullptr;
  digit *head = nullptr;
  int result = 0, firstDigit = 0, secondDigit = 0;

  while(left != nullptr || right != nullptr)
  {
    sum = new digit;
    // if left number has ended
    if(left == nullptr && right != nullptr)
    {
      result = right->data + secondDigit;
    }
    // if right number has ended
    else if(right == nullptr && left != nullptr)
    {
      result = left->data + secondDigit;
    }
    // if both numbers have not ended
    else
    {
      result = left->data + right->data + secondDigit;
    }
    // if result is a double digit number
    if(result > 9)
    {
      firstDigit = result % 10;
      secondDigit = result / 10;
    }
    // if result is a single digit number
    else
    {
      // if empty list
      if(head == nullptr)
      {
        sum->data = result;
        head = sum;
        secondDigit = 0;
      }
      // if list is not empty
      else
      {
        sum->data = result;
        sum->next = head;
        head = sum;
        secondDigit = 0;
      }
    }
    // if secondDigit is not zero (result is double digit)
    if(secondDigit != 0)
    {
      // if list is empty
      if(head == nullptr)
      {
        sum->data = firstDigit;
        head = sum;
      }
      // if list is not empty
      else
      {
        sum->data = firstDigit;
        sum->next = head;
        head = sum;
      }
      // if both lists are about to end (adding a new node for secondDigit)
      if(left->next == nullptr && right->next == nullptr)
      {
        sum = new digit;
        sum->data = secondDigit;
        sum->next = head;
        head = sum;
      }
      // if right number has ended
      if(right == nullptr && left != nullptr)
      {
        sum = new digit;
        sum->data = firstDigit;
        sum->next = head;
        head = sum;
      }
      // if left number has ended
      if(left == nullptr && right != nullptr)
      {
        sum = new digit;
        sum->data = firstDigit;
        sum->next = head;
        head = sum;
      }
    }

    // if right number has ended
    if(right == nullptr && left != nullptr)
    {
      left = left->next;
    }
    // if left number has ended
    else if(left == nullptr && right != nullptr)
    {
      right = right->next;
    }
    // if both numbers have not ended
    else
    {
      left = left->next;
      right = right->next;
    }
  }
  return sum;
}
// =============================================================================

// =============================================================================
// This function will handle the borrowing and carries done in subtraction. There
// are two cases, the first case is if the digit next where head is pointing to
// can be borrowed from. If so the digit pointed to by head will have ten added
// to it and the digit it is borrowing from will have one subtracted by it. Case
// two is if the digit next to head is zero and thus can not be borrowed from. In
// this case two pointers will traverse the list and until a digit that is not
// zero is found. Once this is done the two pointers will borrow and carry unitl
// the head pointer is reached.
void subCarry(digit * head)
{
  digit *headNode = head;
  digit *currentNode = headNode;
  digit *nextNode = nullptr;
  int zeros_until_num = 1;

  // Case 1: headNode->next->data != 0
  if(headNode->next->data != 0)
  {
    nextNode = headNode->next;
    headNode->data = headNode->data + 10;
    nextNode->data = nextNode->data - 1;
    return;
  }

  // Case 2: headNode->next->data == 0
  nextNode = currentNode->next;
  while(nextNode->data == 0)
  {
    currentNode = currentNode->next;
    nextNode = nextNode->next;
    zeros_until_num++;
  }
  while(zeros_until_num != 0)
  {
    currentNode->data = currentNode->data + 10;
    nextNode->data = nextNode->data - 1;
    zeros_until_num--;
    currentNode = headNode;
    nextNode = currentNode->next;
    while(nextNode->data == 0)
    {
      currentNode = currentNode->next;
      nextNode = nextNode->next;
    }
  }
  return;
}
// =============================================================================

// =============================================================================
// This function deteremines which subtraction function to call; once the appropriate
// subtraction is called it will return the pointer pointing to the list containing
// the difference of the left and right numbers.
digit * subNumbers(digit * left, digit * right)
{
  int compareNums = digcmp(left, right);
  digit *difference = nullptr;
  digit *test1 = left;
  digit *test2 = right;

  // Both left and right same size
  if(compareNums == 0)
  {
    // Traversing both the left and right numbers until most significant digit
    while(test1->next != nullptr)
    {
      test1 = test1->next;
    }
    while(test2->next != nullptr)
    {
      test2 = test2->next;
    }
    if(test1->data > test2->data)
    {
      difference = sub_Size_Equal_Left_Greater(left, right);
    }
    else if(test2->data > test1->data)
    {
      difference = sub_Size_Equal_Right_Greater(left, right);
    }
  }
  // Right is larger than left
  else if(compareNums == -1)
  {
    difference = sub_Right_Length_Greater(left, right);
  }
  // Left is larger than right
  else
  {
    difference = sub_Left_Length_Greater(left, right);
  }
  return difference;
}
// =============================================================================

// =============================================================================
// This function is called if both numbers are of equal length, but the number
// pointed to by the left pointer is greater to the right number in the absolute
// sense. It returns the pointer pointing to the list containing the difference.
digit * sub_Size_Equal_Left_Greater(digit * left, digit * right)
{
  digit *difference = nullptr;
  digit *head = nullptr;
  int result = 0;

  while(left != nullptr || right != nullptr)
  {
    difference = new digit;

    result = left->data - right->data;

    // If result is negative, carry and recalculate into positive
    if(result < 0)
    {
      subCarry(left);
      result = left->data - right->data;
    }

    // If list is empty
    if(head == nullptr)
    {
      difference->data = result;
      head = difference;
    }
    // If list is not empty
    else
    {
      difference->data = result;
      difference->next = head;
      head = difference;
    }
    left = left->next;
    right = right->next;
  }
  // If head is pointing to a zero remove it
  if(head->data == 0)
  {
    digit *deleteZero = difference;
    difference = difference->next;
    delete deleteZero;
  }
  return difference;
}
// =============================================================================

// =============================================================================
// This function is called if both numbers are of equal length, but the number
// pointed to by the right pointer is greater to the left number in the absolute
// sense. Since the right number is negative, after the calculation the most
// significant digit is set to negative. It returns the pointer pointing to the
// list containing the difference.
digit * sub_Size_Equal_Right_Greater(digit * left, digit * right)
{
  digit *difference = nullptr;
  digit *head = nullptr;
  int result = 0;

  while(left != nullptr || right != nullptr)
  {
    difference = new digit;

    result = right->data - left->data;

    // If result is a negative, carry and recalculate so positive
    if(result < 0)
    {
      subCarry(right);
      result = right->data - left->data;
    }

    // If list is empty
    if(head == nullptr)
    {
      difference->data = result;
      head = difference;
    }
    // If list is not empty
    else
    {
      difference->data = result;
      difference->next = head;
      head = difference;
    }

    right = right->next;
    left = left->next;
  }
  // If head is pointing to a zero remove it
  if(head->data == 0)
  {
    digit *deleteZero = difference;
    difference = difference->next;
    delete deleteZero;
  }
  // Set most significant digit to negative
  setNeg(difference);

  return difference;
}
// =============================================================================

// =============================================================================
// This function is called when the list pointed to by the right pointer is
// longer than the left number. Since the right number is negative the most
// significant digit is set to negative before the pointer pointing to the
// difference is returned.
digit * sub_Right_Length_Greater(digit * left, digit * right)
{
  digit *difference = nullptr;
  digit *head = nullptr;
  int result = 0;

  while(left != nullptr || right != nullptr)
  {
    difference = new digit;
    // If left number has ended
    if(left == nullptr && right != nullptr)
    {
      result = right->data + 0;
    }
    // If both numbers have not ended
    else
    {
      result = right->data - left->data;
    }

    // If result is a negative number, borrow and recalculate result for positive
    if(result < 0)
    {
      subCarry(right);
      result = right->data - left->data;
    }

    // If the list is empty
    if(head == nullptr)
    {
      difference->data = result;
      head = difference;
    }
    // List is not empty
    else
    {
      difference->data = result;
      difference->next = head;
      head = difference;
    }

    // If the left number has ended, traverse right
    if(left == nullptr && right != nullptr)
    {
      right = right->next;
    }
    // Traverse both the left and right
    else
    {
      right = right->next;
      left = left->next;
    }
  }
  // If head is pointing to a zero remove it
  if(head->data == 0)
  {
    digit *deleteZero = difference;
    difference = difference->next;
    delete deleteZero;
  }
  // Set most significant digit to negative
  setNeg(difference);

  return difference;
}
// =============================================================================

// =============================================================================
// This function is called when the list pointed to by the left pointer is of
// greater length than the right number. It returns a pointer pointing to the
// difference of the left and right numbers.
digit * sub_Left_Length_Greater(digit * left, digit *right)
{
  digit *difference = nullptr;
  digit *head = nullptr;
  int result = 0;

  while(left != nullptr || right != nullptr)
  {
    difference = new digit;

    // If the right number has ended
    if(right == nullptr && left != nullptr)
    {
      result = left->data + 0;
    }
    // If both the left and the right have not ended
    else
    {
      result = left->data - right->data;
    }

    // If result is negative, carry and recalculate
    if(result < 0)
    {
      subCarry(left);
      result = left->data - right->data;
    }

    // If list is empty
    if(head == nullptr)
    {
      difference->data = result;
      head = difference;
    }
    // If list is not empty
    else
    {
      difference->data = result;
      difference->next = head;
      head = difference;
    }

    // If the right has ended, traverse left
    if(right == nullptr && left != nullptr)
    {
      left = left->next;
    }
    // Traverse both the left and the right
    else
    {
      left = left->next;
      right = right->next;
    }
  }
  // If head is pointing to a zero remove it
  if(head->data == 0)
  {
    digit *deleteZero = difference;
    difference = difference->next;
    delete deleteZero;
  }
  return difference;
}
// =============================================================================

// =============================================================================
// This function sets the most significant digit in the difference to a negative.
void setNeg(digit * num)
{
  num->data = num->data - (num->data * 2);
}
// =============================================================================
