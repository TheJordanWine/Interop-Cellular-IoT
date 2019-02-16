/**
 * ValidityCheck Header File:  ValidityCheck.h
 *
 * This file is used to represent the interface for Validty Check class.
 * This class contains several functions that check whether a string or
 * char[] is valid for its intended application.
 *
 * @author Team 6
 *
 * @version 1.0
 */

#ifndef ValidityCheck_H
#define ValidityCheck_H

class ValidityCheck {

  public:

    ValidityCheck();

    bool isValidIP(const char x[]);
    bool isValidCred(const char x[]);
    bool isValidInt(const char x[]);
    bool isValidName(const char x[]);
    bool isValidPath(const char x[]);
    bool isValidPass(const char x[]);

  private:

};

#endif
