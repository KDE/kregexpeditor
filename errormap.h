#ifndef ERRORMAP_H
#define ERRORMAP_H

class ErrorMap 
{
public:
    ErrorMap();
    void start();
    void end();
    void lineStartError();
    void lineEndError();
    void lookAheadError();
private:
    bool _lineStartError, _prevLineStartError;
    bool _lineEndError, _prevLineEndError;
    bool _lookAHeadError, _prevLookAHeadError;
};

#endif
