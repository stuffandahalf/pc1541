#ifndef IDRIVE_H
#define IDRIVE_H

class IDrive {
public:
    virtual int initialize() = 0;
    virtual void execute() = 0;
    
    virtual ~IDrive() { }
};

#endif /* IDrive_h */
