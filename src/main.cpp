// main.cpp

#include <QDomDocument>
#include <QtCore/QtCore>
#include <QTextStream>

#include "simpleType.h"
#include "complexType.h"
#include "settings.h"
#include "objects.h"
#include "utils.h"
#include "generator.h"
#include "normalizer.h"

class Type;

void printTypes() {
	qDebug() << "types";
	QList<Type*> vs = getTypes().values();
	for (int i = 0; i < vs.size(); i++) {
		Type* t = vs.at(i);
		QString s = t->getName();
		s.append(t->hasSimpleContent() ? " *" : " #");
		s.append(" " + QString::number((t->getAttributes()->size())));
		qDebug() << s;
	}
}

void printAttrs() {
	qDebug() << "attrs";
	QList<Attribute*> vs = getAttrs().values();
	for (int i = 0; i < vs.size(); i++) {
		Attribute* t = vs.at(i);
		QString s = t->getName();
		qDebug() << s;
	}
}

void printEles() {
	qDebug() << "elements";
	QList<Element*> vs = getElements().values();
	for (int i = 0; i < vs.size(); i++) {
		Element* t = vs.at(i);
		QString s = t->getName();
		qDebug() << s;
	}
}

QDomDocument * loadXmlDocument(QFile * file) {
	if (!file->exists()) {
		qDebug() << "doesn't exist";
		return NULL;
	}

	QString errorStr;
	int errorLine;
	int errorColumn;

	QDomDocument *doc = new QDomDocument;
	if (!doc->setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
		qDebug()
				<< QString("Error > %1 < in file %2 on line %3, column %4").arg(
						errorStr).arg(file->fileName()).arg(errorLine).arg(
						errorColumn);
		delete doc;
		doc = NULL;
	}
	return doc;
}

void makeGlobalTypes(QDomElement root) {
	QQueue<QDomElement> waitPoints;

	QList<QDomElement> ss = getElements(root, "simpleType");
	for (int i = 0; i < ss.size(); i++) {
		QDomElement s = ss.at(i);
		waitPoints.enqueue(s);
	}

	while (waitPoints.size() > 0) {
		QDomElement s = waitPoints.dequeue();
		if (!simpleType(s)) {
			waitPoints.enqueue(s);
		}
	}

	QList<QDomElement> cs = getElements(root, "complexType");
	for (int i = 0; i < cs.size(); i++) {
		QDomElement c = cs.at(i);
		waitPoints.enqueue(c);
	}
	while (waitPoints.size() > 0) {
		QDomElement c = waitPoints.dequeue();
		if (!complexType(c)) {
			waitPoints.enqueue(c);
		}
	}
}

int main(int argc, char *argv[])
{

    if (argc != (1 + 4))
    {
        QTextStream out(stdout);
        out << "usage"
        << argv[0]
        << "<schema file>"      // argv[1]
        << "<user namesapce>"   // argv[2]
        << "<schema namespace>" // argv[3]
        << "<destination dir>"; // argv[4]

		return EXIT_FAILURE;
	}

	Settings::settings()->setUserPrefix(QString(argv[2]).isEmpty()?QString():QString(argv[2]));
	Settings::settings()->setSchemaPrefix(QString(argv[3]).isEmpty()?QString():QString(argv[3]));
	Settings::settings()->setDir(QString(argv[4]));

	prepareBuildInTypes();

	QFile *in = new QFile(argv[1]);
	QDomDocument *doc = loadXmlDocument(in);
    if ( doc == NULL )
    {
		qDebug() << QString("error");
		return EXIT_FAILURE;
    }
    else
    {
		globalizeAndName(*doc);
		QDomElement root = doc->documentElement();

		QList<QDomElement> els = getAllElements(root);
		for (int i = 0; i < els.size(); i++) {
			qDebug() << els.at(i).attribute("name");
		}

		makeEmptyAttributes(root);
		makeEmptyElements(root);
		makeGlobalTypes(root);
		makeAttributeBowels(root);
		makeElementBowels(root);
		printTypes();
		printAttrs();
		printEles();
		generateClasses();
		delete doc;
	}

	return EXIT_SUCCESS;
}
