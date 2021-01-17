#ifndef QTDEPRECATIONS_H
#define QTDEPRECATIONS_H

#include <QSet>
#include <QList>
#include <QLineF>

namespace Molsketch {

#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
template<class T>
inline QSet<T> toSet(const QList<T>& list) { return QSet<T>::fromList(list); }
inline QLineF::IntersectType intersectionType(const QLineF& first, const QLineF& second, QPointF *intersectionPoint) {
  return first.intersect(second, intersectionPoint);
}
#else
template<class T>
inline QSet<T> toSet(const QList<T>& list) { return QSet<T>(list.cbegin(), list.cend()); }

inline QLineF::IntersectType intersectionType(const QLineF& first, const QLineF& second, QPointF *intersectionPoint) {
  return first.intersects(second, intersectionPoint);
}
#endif
} // namespace Molsketch


#endif // QTDEPRECATIONS_H
