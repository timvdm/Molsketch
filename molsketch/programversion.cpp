#include "programversion.h"

struct ProgramVersion::ProgramVersionPrivate {
  unsigned int major, minor, patch, build;
};

ProgramVersion::ProgramVersion(const QString &version)
  : d(new ProgramVersionPrivate) {
  d->major = version.section('.', 0,0).toInt();
  d->minor = version.section('.', 1,1).toInt();
  d->patch = version.section('.', 2,2).toInt();
  d->build = version.section('.', 3,3).toInt();
}

ProgramVersion::ProgramVersion(int major, int minor, int patch, int build)
  : d(new ProgramVersionPrivate) {
  d->major = major;
  d->minor = minor;
  d->patch = patch;
  d->build = build;
}

ProgramVersion::~ProgramVersion() {
  delete d;
}

unsigned int ProgramVersion::patch() const {
  return d->patch;
}

unsigned int ProgramVersion::build() const {
  return d->build;
}

bool ProgramVersion::operator ==(const ProgramVersion &other) const {
  return d->major == other.d->major
      && d->minor == other.d->minor
      && d->patch == other.d->patch
      && d->build == other.d->build;
}

bool ProgramVersion::operator !=(const ProgramVersion &other) const {
  return ! ((*this) == other);
}

bool ProgramVersion::operator <(const ProgramVersion &other) const {
  return d->major < other.d->major
      || (d->major == other.d->major && (
        d->minor < other.d->minor
        || (d->minor == other.d->minor && (
          d->patch < other.d->patch
          || (d->patch == other.d->patch && d->build < other.d->build)))));
}

bool ProgramVersion::operator >(const ProgramVersion &other) const {
  return !((*this) <= other);
}

bool ProgramVersion::operator <=(const ProgramVersion &other) const {
  return (*this) == other || (*this) < other;
}

bool ProgramVersion::operator >=(const ProgramVersion &other) const {
  return !((*this) < other);
}

QString ProgramVersion::toString() const
{
  return QString("%1.%2.%3.%4").arg(d->major).arg(d->minor).arg(d->patch).arg(d->build);
}

unsigned int ProgramVersion::minor() const {
  return d->minor;
}

unsigned int ProgramVersion::major() const {
  return d->major;
}
