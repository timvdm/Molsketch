#ifndef PROGRAMVERSION_H
#define PROGRAMVERSION_H

#include <QString>



class ProgramVersion
{
public:
  ProgramVersion(const QString& version = "");
  ProgramVersion(int major, int minor = 0, int patch = 0, int build = 0);
  ~ProgramVersion();
  unsigned int major() const;
  unsigned int minor() const;
  unsigned int patch() const;
  unsigned int build() const;
  bool operator ==(const ProgramVersion& other) const;
  bool operator !=(const ProgramVersion& other) const;
  bool operator <(const ProgramVersion& other) const;
  bool operator >(const ProgramVersion& other) const;
  bool operator <=(const ProgramVersion& other) const;
  bool operator >=(const ProgramVersion& other) const;
  QString toString() const;
private:
  class ProgramVersionPrivate;
  ProgramVersionPrivate *d;
};

#endif // PROGRAMVERSION_H
