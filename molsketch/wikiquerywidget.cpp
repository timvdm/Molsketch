/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
 *   HVennekate@gmx.de                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "wikiquerywidget.h"
#include "ui_wikiquerywidget.h"
#include "molecule.h"
#include "obabeliface.h"

#include <QJsonDocument>
#include <QLibrary>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <librarymodel.h>
#include <moleculemodelitem.h>

#ifdef _WIN32
#define OBABELOSSUFFIX ".dll"
#else
#define OBABELOSSUFFIX
#endif

using namespace Molsketch;

WikiQueryWidget::WikiQueryWidget(QWidget *parent) :
  QDockWidget(parent),
  ui(new Ui::WikiQueryWidget),
  manager(new QNetworkAccessManager(this))
{
  ui->setupUi(this);
  ui->moleculeListView->setModel(new LibraryModel(this));
  connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processMoleculeQuery(QNetworkReply*)));
  ui->progressWidget->hide();
  ui->queryInput->setValidator(new QRegularExpressionValidator(QRegularExpression("[^\"]*"),
                                                               ui->queryInput));
  // TODO validator for input: do not allow quotation marks. Check input constraints
}

WikiQueryWidget::~WikiQueryWidget()
{
  delete ui;
}

void WikiQueryWidget::on_searchButton_clicked() {
  startMoleculeQuery(ui->queryInput->text());
}

void WikiQueryWidget::on_queryInput_textChanged(const QString &newText) {
  ui->searchButton->setDisabled(newText.length() < 5);
}

void WikiQueryWidget::processMoleculeQuery(QNetworkReply *reply) {
  ui->progressWidget->hide();
  if (!reply->isFinished()) reply->abort();
  if (!reply) return;

  QLibrary obabeliface("obabeliface" QTVERSIONSUFFIX OBABELOSSUFFIX);
  obabeliface.load() ;
  fromInChIFunctionPointer convertInChI = 0 ;
  if (obabeliface.isLoaded())
    convertInChI = (fromInChIFunctionPointer) (obabeliface.resolve("fromInChI")) ;
  if (!convertInChI) {
    qWarning("Could not load function to convert InChI");
    QMessageBox::critical(0, "Error", "Could not load OpenBabel function to convert InChI.");
    return;
  }

  QJsonDocument response = QJsonDocument::fromJson(reply->readAll());

  QVariantList results = response.toVariant().toMap()["results"].toMap()["bindings"].toList();

  QList<QPair<QString, QString>> namesAndInchiStrings;
  foreach (QVariant result, results) {
    QString label = result.toMap()["label"].toMap()["value"].toString();
    QString smiles = result.toMap()["smiles"].toMap()["value"].toString();
    QString inchi = result.toMap()["inchi"].toMap()["value"].toString();
    QString isomer = result.toMap()["isomer"].toMap()["value"].toString();
    qInfo() << "obtained molecule from wikidata:" << label << smiles << inchi << isomer;
    namesAndInchiStrings << qMakePair(label, inchi);
  }
  qSort(namesAndInchiStrings);
  QList<MoleculeModelItem*> items;
  for (auto item : namesAndInchiStrings)
    items << MoleculeModelItem::fromInChI(item.first, item.second);
  ((LibraryModel*) ui->moleculeListView->model())->setMolecules(items);
  reply->deleteLater();
}

void WikiQueryWidget::startMoleculeQuery(const QString& queryString) {
  QUrlQuery query;
  query.setQueryItems(QList<QPair<QString, QString> >() <<
                      qMakePair(QString("query"),
                                "SELECT (SAMPLE(?label) AS ?label)"
                                " (SAMPLE(?smiles) AS ?smiles)"
                                " (SAMPLE(?inchi) AS ?inchi)"
                                " (SAMPLE(?isomer) AS ?isomer) WHERE"
                                " { ?qnumber wdt:P31 wd:Q11173"
                                " . ?qnumber rdfs:label ?label"
                                " . ?qnumber wdt:P233 ?smiles"
                                " . ?qnumber wdt:P234 ?inchi"
                                " . ?qnumber wdt:P2017 ?isomer"
                                " . SERVICE wikibase:label { bd:serviceParam wikibase:language \"en\". } "
                                "FILTER(REGEX(?label, \"" + queryString + "\", \"i\")) "
                                "} GROUP BY ?qnumber"
//                                " LIMIT 100 ORDER BY DESC(?label)"
                                ));
  QUrl url("https://query.wikidata.org/sparql");
  url.setQuery(query);
  QNetworkRequest request(url);
  request.setRawHeader("Accept", "application/json");
  QNetworkReply *reply = manager->get(request);
  connect(ui->stopButton, SIGNAL(clicked(bool)), reply, SLOT(abort()));
  ui->progressWidget->show();
}
