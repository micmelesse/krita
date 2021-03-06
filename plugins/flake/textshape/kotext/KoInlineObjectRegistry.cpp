/* This file is part of the KDE project
 * Copyright (C) 2006-2010 Thomas Zander <zander@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "KoInlineObjectRegistry.h"
#include "KoInlineObjectFactoryBase.h"
#include "InsertVariableAction_p.h"

#include <KoCanvasBase.h>
#include <KoInlineObject.h>
#include <KoXmlReader.h>
#include <KoPluginLoader.h>

#include <QGlobalStatic>

#include "TextDebug.h"

Q_GLOBAL_STATIC(KoInlineObjectRegistry, s_instance)


class Q_DECL_HIDDEN KoInlineObjectRegistry::Private
{
public:
    void insertFactory(KoInlineObjectFactoryBase *factory);
    void init(KoInlineObjectRegistry *q);

    QHash<QPair<QString, QString>, KoInlineObjectFactoryBase *> factories;
};

void KoInlineObjectRegistry::Private::init(KoInlineObjectRegistry *q)
{
    KoPluginLoader::PluginsConfig config;
    config.whiteList = "TextInlinePlugins";
    config.blacklist = "TextInlinePluginsDisabled";
    config.group = "krita";
    KoPluginLoader::instance()->load(QString::fromLatin1("Krita/Text-InlineObject"),
                                     QString::fromLatin1("[X-KoText-PluginVersion] == 28"), config);

    foreach (KoInlineObjectFactoryBase *factory, q->values()) {
        QString nameSpace = factory->odfNameSpace();
        if (nameSpace.isEmpty() || factory->odfElementNames().isEmpty()) {
            debugText << "Variable factory" << factory->id() << " does not have odfNameSpace defined, ignoring";
        } else {
            foreach (const QString &elementName, factory->odfElementNames()) {
                factories.insert(QPair<QString, QString>(nameSpace, elementName), factory);

                debugText << "Inserting variable factory" << factory->id() << " for"
                    << nameSpace << ":" << elementName;
            }
        }
    }
}

KoInlineObjectRegistry* KoInlineObjectRegistry::instance()
{
    if (!s_instance.exists()) {
        s_instance->d->init(s_instance);
    }
    return s_instance;
}

QList<QAction*> KoInlineObjectRegistry::createInsertVariableActions(KoCanvasBase *host) const
{
    QList<QAction*> answer;
    foreach (const QString &key, keys()) {
        KoInlineObjectFactoryBase *factory = value(key);
        if (factory->type() == KoInlineObjectFactoryBase::TextVariable) {
            foreach (const KoInlineObjectTemplate &templ, factory->templates()) {
                answer.append(new InsertVariableAction(host, factory, templ));
            }
#ifndef NDEBUG
           if (factory->templates().isEmpty()) {
                warnText << "Variable factory" << factory->id() << "has no templates, skipping.";
           }
#endif
        }
    }
    return answer;
}

KoInlineObject *KoInlineObjectRegistry::createFromOdf(const KoXmlElement &element, KoShapeLoadingContext &context) const
{
    KoInlineObjectFactoryBase *factory = d->factories.value(
            QPair<QString, QString>(element.namespaceURI(), element.tagName()));
    if (factory == 0) {
        debugText << "No factory for" << element.namespaceURI() << ":" << element.tagName();
        return 0;
    }

    KoInlineObject *object = factory->createInlineObject(0);
    if (object) {
        object->loadOdf(element, context);
    }

    return object;
}

KoInlineObjectRegistry::~KoInlineObjectRegistry()
{
    qDeleteAll(doubleEntries());
    qDeleteAll(values());
    delete d;
}

KoInlineObjectRegistry::KoInlineObjectRegistry()
        : d(new Private())
{
}
