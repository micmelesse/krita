/*
 * This file is part of the KDE project
 *
 * Copyright (c) Michael Thaler <michael.thaler@physik.tu-muenchen.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _KIS_SMALL_TILES_FILTER_H_
#define _KIS_SMALL_TILES_FILTER_H_

#include "kis_filter.h"
#include "kis_filter_config_widget.h"

class KisSmallTilesFilterConfiguration : public KisFilterConfiguration
{
public:
    KisSmallTilesFilterConfiguration(Q_UINT32 numberOfTiles) : m_numberOfTiles(numberOfTiles) {};

public:
    inline Q_UINT32 numberOfTiles() { return m_numberOfTiles; };

private:
    Q_UINT32 m_numberOfTiles;
};

class KisSmallTilesFilter : public KisFilter
{

public:
    KisSmallTilesFilter();

public:
    virtual void process(KisPaintDeviceImplSP,KisPaintDeviceImplSP, KisFilterConfiguration* , const QRect&);
    static inline KisID id() { return KisID("smalltiles", i18n("Small tiles")); };
    virtual bool supportsPainting() { return true; }
    virtual bool supportsPreview() { return true; }
    virtual std::list<KisFilterConfiguration*> listOfExamplesConfiguration(KisPaintDeviceImplSP )
    { std::list<KisFilterConfiguration*> list; list.insert(list.begin(), new KisSmallTilesFilterConfiguration(2)); return list; }

public:
    virtual KisFilterConfigWidget * createConfigurationWidget(QWidget* parent, KisPaintDeviceImplSP dev);
    virtual KisFilterConfiguration* configuration(QWidget*, KisPaintDeviceImplSP dev);

private:
    void createSmallTiles(KisPaintDeviceImplSP src, KisPaintDeviceImplSP dst, const QRect& rect, Q_UINT32 numberOfTiles);
};

#endif
