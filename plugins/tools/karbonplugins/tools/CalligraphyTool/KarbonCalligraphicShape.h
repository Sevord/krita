/* This file is part of the KDE project
   Copyright (C) 2008 Fela Winkelmolen <fela.kde@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
*/

#ifndef KARBONCALLIGRAPHICSHAPE_H
#define KARBONCALLIGRAPHICSHAPE_H

#include <KoParameterShape.h>
#include <kis_paint_information.h>

#define KarbonCalligraphicShapeId "KarbonCalligraphicShape"

class KarbonCalligraphicPoint
{
public:
    KarbonCalligraphicPoint(KisPaintInformation &paintInfo)
        : m_paintInfo(paintInfo) {}

    QPointF point() const
    {
        return m_paintInfo.pos();
    }
    qreal angle() const
    {
        return (fmod((m_paintInfo.drawingAngle()* 180.0 / M_PI)+90.0, 360.0)* M_PI / 180);
    }
    qreal width() const
    {
        return m_paintInfo.pressure()*20.0;
    }

    KisPaintInformation paintInfo()
    {
        return m_paintInfo;
    }

    void setPaintInfo(KisPaintInformation &paintInfo)
    {
        m_paintInfo = paintInfo;
    }

    void setPoint(const QPointF &point)
    {
        m_paintInfo.setPos(point);
    }

private:
    KisPaintInformation m_paintInfo;
};

/*class KarbonCalligraphicShape::Point
{
public:
    KoPainterPath(KoPathPoint *point) : m_prev(point), m_next(0) {}

    // calculates the effective point
    QPointF point() {
        if (m_next = 0)
            return m_prev.point();

        // m_next != 0
        qDebug() << "not implemented yet!!!!";
        return QPointF();
    }

private:
    KoPainterPath m_prev;
    KoPainterPath m_next;
    qreal m_percentage;
};*/

// the indexes of the path will be similar to:
//        7--6--5--4   <- pointCount() / 2
// start  |        |   end    ==> (direction of the stroke)
//        0--1--2--3
class KarbonCalligraphicShape : public KoParameterShape
{
public:
    explicit KarbonCalligraphicShape(qreal caps = 0.0);
    ~KarbonCalligraphicShape();

    KoShape* cloneShape() const override;

    void appendPoint(KisPaintInformation &paintInfo);
    void appendPointToPath(const KarbonCalligraphicPoint &p);

    KarbonCalligraphicPoint* lastPoint();

    // returns the bounding rect of whan needs to be repainted
    // after new points are added
    const QRectF lastPieceBoundingRect();

    void setSize(const QSizeF &newSize);
    //virtual QPointF normalize();

    QPointF normalize();

    void simplifyPath();

    void simplifyGuidePath();

    // reimplemented
    virtual QString pathShapeId() const;

protected:
    // reimplemented
    void moveHandleAction(int handleId,
                          const QPointF &point,
                          Qt::KeyboardModifiers modifiers = Qt::NoModifier);

    // reimplemented
    void updatePath(const QSizeF &size);

private:
    KarbonCalligraphicShape(const KarbonCalligraphicShape &rhs);

    // auxiliary function that actually insererts the points
    // without doing any additional checks
    // the points should be given in canvas coordinates
    void appendPointsToPathAux(const QPointF &p1, const QPointF &p2);

    // function to detect a flip, given the points being inserted
    bool flipDetected(const QPointF &p1, const QPointF &p2);

    void smoothLastPoints();
    void smoothPoint(const int index);

    // determine whether the points given are in counterclockwise order or not
    // returns +1 if they are, -1 if they are given in clockwise order
    // and 0 if they form a degenerate triangle
    static int ccw(const QPointF &p1, const QPointF &p2, const QPointF &p3);

    //
    void addCap(int index1, int index2, int pointIndex, bool inverted = false);

    // the actual data then determines it's shape (guide path + data for points)
    KisDistanceInformation *m_strokeDistance;
    QList<KarbonCalligraphicPoint *> m_points;
    bool m_lastWasFlip;
    qreal m_caps;
};

#endif // KARBONCALLIGRAPHICSHAPE_H

