#include "language/languages.h"

//#include <string>
#include "utilities/stringutil.h"
#include "utilities/filedownloader.h"
#include "qyamlcpp/qyamlcpp.h"

//====================================================================
//=== BCP47Language
//====================================================================
BCP47Language::BCP47Language()
  : m_type(LANGUAGE)
  , m_macrolanguage(false)
  , m_collection(false)
  , m_deprecated(false)
{
}

void
BCP47Language::setSubtag(const QString& tag)
{
  m_subtag = tag;
}

QString
BCP47Language::subtag() const
{
  return m_subtag;
}

void
BCP47Language::addDescription(const QString& desc)
{
  m_descriptions.append(desc);
}

void
BCP47Language::appendDescription(const QString& desc)
{
  if (m_descriptions.length() == 0)
    m_descriptions.append(desc);
  else {
    int i = m_descriptions.size() - 1;
    QString description = m_descriptions.at(i);
    description += "\n";
    description += desc;
    m_descriptions.replace(i, description);
  }
}

QString
BCP47Language::description() const
{
  if (!m_descriptions.isEmpty())
    return m_descriptions.first();
  return QString();
}

QVector<QString>
BCP47Language::descriptions() const
{
  return m_descriptions;
}

void
BCP47Language::setDateAdded(const QDate& date)
{
  m_added = date;
}

QDate
BCP47Language::dateAdded() const
{
  return m_added;
}

void
BCP47Language::setSuppressScript(const QString& lang)
{
  m_suppressScriptLang = lang;
}

QString
BCP47Language::suppressScriptLang() const
{
  return m_suppressScriptLang;
}

bool
BCP47Language::hasSuppressScriptLang() const
{
  return (!m_suppressScriptLang.isEmpty());
}

void
BCP47Language::setMacrolanguageName(const QString& macrolang)
{
  m_macrolanguageName = macrolang;
}

QString
BCP47Language::macrolanguageName() const
{
  return m_macrolanguageName;
}

void
BCP47Language::setCollection(bool collection)
{
  m_collection = collection;
}

bool
BCP47Language::isCollection() const
{
  return m_collection;
}

void
BCP47Language::setMacrolanguage(bool isMacrolanguage)
{
  m_macrolanguage = isMacrolanguage;
}

bool
BCP47Language::isMacrolanguage() const
{
  return m_macrolanguage;
}

QString
BCP47Language::comments() const
{
  return m_comments;
}

void
BCP47Language::setComments(const QString& comments)
{
  m_comments = comments;
}

void
BCP47Language::appendComment(const QString& extra)
{
  m_comments.append("\n");
  m_comments.append(extra);
}

bool
BCP47Language::hasComment()
{
  return (!m_comments.isEmpty());
}

QString
BCP47Language::preferredValue() const
{
  return m_preferredValue;
}

void
BCP47Language::setPreferredValue(const QString& preferredValue)
{
  m_preferredValue = preferredValue;
}

bool
BCP47Language::hasPreferredValue()
{
  return !m_preferredValue.isEmpty();
}

bool
BCP47Language::isDeprecated() const
{
  return m_deprecated;
}

void
BCP47Language::setDeprecated(bool deprecated)
{
  m_deprecated = deprecated;
}

QVector<QString>
BCP47Language::prefix() const
{
  return m_prefix;
}

void
BCP47Language::addPrefix(const QString& prefix)
{
  m_prefix.append(prefix);
}

QString
BCP47Language::tag() const
{
  return m_tag;
}

void
BCP47Language::setTag(const QString& tag)
{
  m_tag = tag;
}

BCP47Language::Type
BCP47Language::fromString(const QString& name)
{
  if (name.toLower() == "language")
    return LANGUAGE;
  else if (name.toLower() == "extlang")
    return EXTLANG;
  else if (name.toLower() == "script")
    return SCRIPT;
  else if (name.toLower() == "region")
    return REGION;
  else if (name.toLower() == "variant")
    return VARIANT;
  else if (name.toLower() == "grandfathered")
    return GRANDFATHERED;
  else if (name.toLower() == "redundant")
    return REDUNDANT;
  return BAD_TAG;
}

BCP47Language::Type
BCP47Language::type() const
{
  return m_type;
}

void
BCP47Language::setType(const Type& type)
{
  m_type = type;
}

void
BCP47Language::setTypeFromString(const QString&)
{
  // TODO
}

QString
BCP47Language::typeString()
{
  switch (m_type) {
    case LANGUAGE:
      return "language";
    case EXTLANG:
      return "extlang";
    case SCRIPT:
      return "script";
    case REGION:
      return "region";
    case VARIANT:
      return "variant";
    case GRANDFATHERED:
      return "grandfathered";
    case REDUNDANT:
      return "redundant";
    default:
      return QString();
  }
  return QString();
}

//====================================================================
//=== BCP47Languages
//====================================================================
const QVector<QString> BCP47Languages::TAGTYPES = QVector<QString>()
                                                  << "type"
                                                  << "tag"
                                                  << "subtag"
                                                  << "description"
                                                  << "added"
                                                  << "suppress-script"
                                                  << "prefix"
                                                  << "macrolanguage"
                                                  << "deprecated"
                                                  << "preferred-value"
                                                  << "scope"
                                                  << "comments";
const QString BCP47Languages::IAINREGISTRY =
  "https://www.iana.org/assignments/language-subtag-registry/"
  "language-subtag-registry";

QMultiMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_datasetByDescription =
  QMultiMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_languageByDescription =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_languageBySubtag =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_extlangByDescription =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_extlangBySubtag =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_regionByDescription =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_regionBySubtag =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_scriptByDescription =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_scriptBySubtag =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_variantByDescription =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_variantBySubtag =
  QMap<QString, QSharedPointer<BCP47Language>>();
// some grandfathered descriptions are NOT unique.
QMultiMap<QString, QSharedPointer<BCP47Language>>BCP47Languages::m_grandfatheredByDescription =
  QMultiMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_grandfatheredByTag =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_redundantByDescription =
  QMap<QString, QSharedPointer<BCP47Language>>();
QMap<QString, QSharedPointer<BCP47Language>> BCP47Languages::m_redundantByTag =
  QMap<QString, QSharedPointer<BCP47Language>>();


BCP47Languages::BCP47Languages(QObject* parent)
  : QObject(parent)
  , m_registryName(IAINREGISTRY)
  , m_unStatistical(new UNStatisticalCodes())
{
}

void
BCP47Languages::saveToLocalFile(const QString& filename)
{
  QFile file(filename);
  if (file.open((QFile::ReadWrite | QFile::Truncate))) {
    // remove non-unique languages. (Those with multiple descriptions)
    QVector<QSharedPointer<BCP47Language>> uniqueLanguages =
      getUniqueLanguages();

    YAML::Emitter emitter;
    QString value;
    emitter << YAML::Comment(tr(
      "BCP47 is a conversion of the IANA Language Subtag Registry\n"
      "into a more user friendly form. For the original file see:\n"
      "https://www.iana.org/assignments/language-subtag-registry/"
      "language-subtag-registry.\n\n"
      "This file was generated automatically from the IANA Language Subtag\n"
      "Registry!\n"
      "You should not modify this file by hand as it is VERY easy\n"
      "make a mistake. You should delete the file and regenerate using\n"
      "the BCP47Languages::rebuildFromRegistry() method if the file becomes\n"
      "corrupted or outdated.\n\n"));
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "file-date" << YAML::Value
            << fileDate().toString(Qt::ISODate);
    emitter << YAML::Key << "languages" << YAML::Value;
    emitter << YAML::BeginSeq;
    for (auto& language : uniqueLanguages) {
      emitter << YAML::BeginMap;
      //      emitter << YAML::Key << "COUNT" << YAML::Value <<
      //      language->count();
      emitter << YAML::Key << "type" << YAML::Value << language->typeString();
      value = language->subtag();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "subtag" << YAML::Value << value;
      }
      value = language->tag();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "tag" << YAML::Value << value;
      }
      auto descriptions = language->descriptions();
      if (!descriptions.isEmpty()) {
        emitter << YAML::Key << "description" << YAML::Value;
        emitter << YAML::BeginSeq;
        for (auto& description : descriptions) {
          emitter << description.toStdString().c_str();
        }
        emitter << YAML::EndSeq;
      }
      value = language->dateAdded().toString(Qt::ISODate);
      if (!value.isEmpty()) {
        emitter << YAML::Key << "added" << YAML::Value << value;
      }
      value = language->suppressScriptLang();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "suppress-script" << YAML::Value << value;
      }
      value = language->macrolanguageName();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "macrolanguage" << YAML::Value << value;
      }
      value = language->preferredValue();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "preferred-value" << YAML::Value << value;
      }
      auto prefixes = language->prefix();
      if (!prefixes.isEmpty()) {
        emitter << YAML::Key << "prefix" << YAML::Value;
        emitter << YAML::BeginSeq;
        for (auto& prefix : prefixes) {
          emitter << prefix.toStdString().c_str();
        }
        emitter << YAML::EndSeq;
      }
      if (language->isMacrolanguage()) {
        emitter << YAML::Key << "scope" << YAML::Value << "macrolanguage";
      }
      if (language->isCollection()) {
        emitter << YAML::Key << "scope" << YAML::Value << "collection";
      }
      if (language->isDeprecated()) {
        emitter << YAML::Key << "scope" << YAML::Value << "deprecated";
      }
      if (!language->comments().isEmpty()) {
        emitter << YAML::Key << "comments" << YAML::Value
                << language->comments();
      }
      emitter << YAML::EndMap;
    }
    emitter << YAML::EndSeq;
    emitter << YAML::EndMap;

    QTextStream out(&file);
    out << emitter.c_str();
    file.close();
  }
}

void
BCP47Languages::loadYamlFile(QFile& file)
{
  auto yaml = YAML::LoadFile(file);
  if (yaml["file-date"]) {
    auto node = yaml["file-date"];
    m_fileDate = QDate::fromString(node.as<QString>(), Qt::ISODate);
  }
  if (yaml["languages"]) {
    auto languagesNode = yaml["languages"];
    if (languagesNode && languagesNode.IsSequence()) {
      for (auto languageNode : languagesNode) {
        if (languageNode && languageNode.IsMap()) {
          auto language = QSharedPointer<BCP47Language>(new BCP47Language());
          if (languageNode["type"]) {
            auto value = languageNode["type"].as<QString>();
            language->setType(BCP47Language::fromString(value));
          }
          if (languageNode["subtag"]) {
            auto value = languageNode["subtag"].as<QString>();
            language->setSubtag(value);
          }
          if (languageNode["tag"]) {
            auto value = languageNode["tag"].as<QString>();
            language->setTag(value);
          }
          if (languageNode["added"]) {
            auto value = languageNode["added"].as<QString>();
            language->setDateAdded(QDate::fromString(value, Qt::ISODate));
          }
          if (languageNode["suppress-script"]) {
            auto value = languageNode["suppress-script"].as<QString>();
            language->setSuppressScript(value);
          }
          if (languageNode["macrolanguage"]) {
            auto value = languageNode["macrolanguage"].as<QString>();
            language->setMacrolanguageName(value);
          }
          if (languageNode["preferred-value"]) {
            auto value = languageNode["preferred-value"].as<QString>();
            language->setPreferredValue(value);
          }
          if (languageNode["scope"]) {
            auto value = languageNode["scope"].as<QString>();
            if (value == "deprecated")
              language->setDeprecated(true);
            else if (value == "collection")
              language->setCollection(true);
            else if (value == "macrolanguage")
              language->setMacrolanguage(true);
          }
          auto descriptions = languageNode["description"];
          if (descriptions && descriptions.IsSequence()) {
            for (auto const& description : descriptions) {
              language->addDescription(description.as<QString>());
            }
          }
          auto prefixes = languageNode["prefix"];
          if (prefixes && prefixes.IsSequence()) {
            for (auto const& prefix : prefixes) {
              language->addPrefix(prefix.as<QString>());
            }
          }
          auto comments = languageNode["comments"];
          if (comments && comments.IsScalar()) {
            language->setComments(comments.as<QString>());
          }
          // save language data (multi language description supported)
          for (auto& description : language->descriptions()) {
            addLanguage(description, language);
          }
        }
      }
    }
  }
  updateMaps();
}

void
BCP47Languages::updateMaps()
{
  m_languageBySubtag.clear();
  m_extlangBySubtag.clear();
  m_regionBySubtag.clear();
  m_scriptBySubtag.clear();
  m_variantBySubtag.clear();
  m_grandfatheredByTag.clear();
  m_redundantByTag.clear();

  auto uniqueDescriptions = m_datasetByDescription.uniqueKeys();
  for (auto& description : uniqueDescriptions) {
    auto languages = m_datasetByDescription.values(description);
    for (auto& language : languages) {
      auto subtag = language->subtag();
      auto tag = language->tag();
      auto type = language->type();
      switch (type) {
        case BCP47Language::LANGUAGE:
          m_languageByDescription.insert(description, language);
          m_languageBySubtag.insert(subtag, language);
          //            m_datasetBySubtag.insert(subtag, language);
          break;
        case BCP47Language::EXTLANG:
          m_extlangByDescription.insert(description, language);
          m_extlangBySubtag.insert(subtag, language);
          //          m_datasetBySubtag.insert(subtag, language);
          break;
        case BCP47Language::REGION:
          m_regionByDescription.insert(description, language);
          m_regionBySubtag.insert(subtag, language);
          //          m_datasetBySubtag.insert(subtag, language);
          break;
        case BCP47Language::SCRIPT:
          m_scriptByDescription.insert(description, language);
          m_scriptBySubtag.insert(subtag, language);
          //          m_datasetBySubtag.insert(subtag, language);
          break;
        case BCP47Language::VARIANT:
          m_variantByDescription.insert(description, language);
          m_variantBySubtag.insert(subtag, language);
          //          m_datasetBySubtag.insert(subtag, language);
          break;
        case BCP47Language::GRANDFATHERED:
          m_grandfatheredByDescription.insert(description, language);
          m_grandfatheredByTag.insert(tag, language);
          //          m_datasetBySubtag.insert(tag, language);
          break;
        case BCP47Language::REDUNDANT:
          m_redundantByDescription.insert(description, language);
          m_redundantByTag.insert(tag, language);
          //          m_datasetBySubtag.insert(tag, language);
          break;
        default:
          break;
      }
    }
  }
}

// void
// BCP47Languages::checkLocalFileForNewer(
//     const QString &filename,
//   QMultiMap<QString, QSharedPointer<BCP47Language>> map,
//   QDate fileDate)
//{
//   if (m_fileDate < fileDate) {
//     m_datasetByDescription = map;
//     updateMaps();
//     emit languagesReset();
//     saveToLocalFile(filename);
//   }
// }

void
BCP47Languages::reloadData()
{
  //  auto data = m_thread->languagesData();

  //  m_languages = data;
  //  updateMaps();
  //  emit languagesReset();
  //  saveToLocalFile(m_languageFile);

  //  m_thread->stop();
  //  m_thread->deleteLater();
  //  m_thread = nullptr;
}

void
BCP47Languages::readFromLocalFile(const QString& filename)
{
  QFile file(filename);
  if (file.exists()) {
    loadYamlFile(file);
    emit completed();
  }

  // reloads the language data from the registry in the background,
  // and checks the file date. If the local language file doesn't exist
  // or it is outdated then the file will be replaced with new data.
  rebuildFromRegistry();
}

void
BCP47Languages::rebuildFromRegistry()
{
  QThread* thread = new QThread;
  auto worker = new FileDownloader();
  worker->setDownloadUrl(m_registryName);
  worker->moveToThread(thread);
  connect(worker, &FileDownloader::error, this, &BCP47Languages::errorReceived);
  connect(
    worker, &FileDownloader::dataDownloaded, this, &BCP47Languages::parseData);
  connect(thread, &QThread::started, worker, &FileDownloader::download);
  connect(worker, &FileDownloader::finished, thread, &QThread::quit);
  connect(
    worker, &FileDownloader::finished, worker, &FileDownloader::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  thread->start();
}

void
BCP47Languages::setRegistry(const QString& registry)
{
  m_registryName = registry;
}

void
BCP47Languages::errorReceived(const QString& errorStr)
{
  emit error(errorStr);
}

void
BCP47Languages::parsingErrorsReceived(
  QMultiMap<int, LanguageParser::Errors> errors)
{
  // TODO parse errors and send message
  QString message;
  if (!errors.isEmpty()) {
    auto keys = errors.keys();
    for (auto& line : keys) {
      auto error = errors.value(line);
      if (error.testFlag(LanguageParser::NO_ERROR)) {
        // This should never happen, but just in case
        message.append(tr("No Errors"));
      }
      if (error.testFlag(LanguageParser::BAD_FILE_DATE)) {
        if (message.isEmpty()) {
          message.append(",");
        }
        message.append(
          tr("The file date \"%1\"does not exist. Possibly corrupt file!"));
      }
      if (error.testFlag(LanguageParser::EMPTY_NAME)) {
        if (message.isEmpty()) {
          message.append(",");
        }
        if (error.testFlag(LanguageParser::EMPTY_VALUE)) {
          message.append(tr("Bad data line. Has \":\" but no name or value!"));
        } else {
          message.append(tr("Bad data line. Has \":\" but missing value!"));
        }
      }
      if (error.testFlag(LanguageParser::EMPTY_VALUE)) {
        if (error.testFlag(LanguageParser::EMPTY_NAME)) {
          // already handles in EMPTY_NAME test
          continue;
        }
        message.append(tr("Bad data line. Has \":\" but missing name!"));
      }
      if (error.testFlag(LanguageParser::UNKNOWN_TAG_TYPE)) {
        if (message.isEmpty()) {
          message.append(",");
        }
        message.append(tr("The tag type is not a valid type!"));
      }
      message.prepend(tr("Line %1: ").arg(line));
      message.append("\n");
    }
    emit parsingError(message);
  }
}

void
BCP47Languages::parseData(const QByteArray& data)
{
  QThread* thread = new QThread;
  worker = new LanguageParser();
  worker->setData(data);
  worker->moveToThread(thread);
  connect(worker,
          &LanguageParser::parsingErrors,
          this,
          &BCP47Languages::parsingErrorsReceived);
  connect(worker,
          &LanguageParser::parseCompleted,
          this,
          &BCP47Languages::iainFileParsed);
  connect(thread, &QThread::started, worker, &LanguageParser::parse);
  connect(worker, &LanguageParser::finished, thread, &QThread::quit);
  connect(
    worker, &LanguageParser::finished, worker, &LanguageParser::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  thread->start();
}

QVector<QSharedPointer<BCP47Language>>
BCP47Languages::getUniqueLanguages()
{
  QVector<QSharedPointer<BCP47Language>> uniqueLanguages;
  for (auto& language : m_datasetByDescription.values()) {
    if (!uniqueLanguages.contains(language)) {
      uniqueLanguages.append(language);
    }
  }
  return uniqueLanguages;
}

void
BCP47Languages::iainFileParsed(
  QMultiMap<QString, QSharedPointer<BCP47Language>> languages,
  QDate fileDate,
  bool noErrors)
{
  if (m_fileDate < fileDate) {
    if (noErrors) {
      m_fileDate = fileDate;
      m_datasetByDescription = languages;
      saveToLocalFile(m_languageFilename);
      emit sendMessage(
        tr("Language file updated %1").arg(m_fileDate.toString(Qt::ISODate)));
    } else {
      emit error(tr("The registry file had errors!"));
    }
  }
}

QVector<QString>
BCP47Languages::scriptDescriptions() const
{
  return m_scriptByDescription.keys();
}

QVector<QString>
BCP47Languages::scriptSubtags() const
{
  return m_scriptBySubtag.keys();
}

QString
BCP47Languages::scriptTag(const QString& languageName,
                          const QString& scriptName)
{
  auto tags = m_languageBySubtag.value(languageName);
  auto scriptTag = m_scriptBySubtag.value(scriptName);
  if (!tags.isNull() || !scriptTag.isNull())
    return tags->prefix().at(0) + "-" + scriptTag->subtag() + "-" +
           tags->subtag();
  return QString();
}

QString
BCP47Languages::variantTag(const QString& scriptName, const QString& region)
{
  auto tags = m_variantBySubtag.value(scriptName);
  if (region.isEmpty()) {
    if (!tags.isNull())
      return tags->prefix().at(0) + "-" + tags->subtag();
  } else {
    auto regTag = m_regionBySubtag.value(region);
    if (!tags.isNull() || !regTag.isNull())
      return tags->prefix().at(0) + "-" + regTag->subtag() + "-" +
             tags->subtag();
  }
  return QString();
}

QVector<QString>
BCP47Languages::grandfatheredDescriptions() const
{
  return m_grandfatheredByDescription.keys();
}

QVector<QString>
BCP47Languages::grandfatheredTags() const
{
  return m_grandfatheredByTag.keys();
}

bool
BCP47Languages::isType(const QString& type)
{
  if (TAGTYPES.contains(type, Qt::CaseInsensitive)) {
    return true;
  }
  return false;
}

BCP47Language::Type
BCP47Languages::typeFromString(const QString& value)
{
  if (isPrimaryLanguage(value))
    return BCP47Language::LANGUAGE;
  else if (isExtLang(value))
    return BCP47Language::EXTLANG;
  else if (isVariant(value))
    return BCP47Language::VARIANT;
  else if (isRegion(value))
    return BCP47Language::REGION;
  else if (isScript(value))
    return BCP47Language::SCRIPT;
  else if (isGrandfathered(value))
    return BCP47Language::GRANDFATHERED;
  else if (isRedundant(value))
    return BCP47Language::REDUNDANT;
  return BCP47Language::BAD_TAG;
}

QVector<QSharedPointer<BCP47Language::TagTestResult>>
BCP47Languages::testTag(QString& tag)
{
  QVector<QSharedPointer<BCP47Language::TagTestResult>> results;
  QSharedPointer<BCP47Language::TagTestResult> result;

  // remove all spaces.
  auto testValue = StringUtil::removeWhitespace(tag);

  QString subvalue;

  for (int i = 0; i < testValue.length(); i++) {
    auto c = testValue.at(i);
    auto pos = i + 1;
    if (c != '-' && pos <= testValue.length()) {
      subvalue += c;
      if (!result) {
        result = QSharedPointer<BCP47Language::TagTestResult>(
          new BCP47Language::TagTestResult());
        result->start = i;
      }
    }

    if (c == '-' || pos == testValue.length()) {
      auto type = checkPrimaryLanguage(subvalue);
      if (type != BCP47Language::NO_PRIMARY_LANGUAGE) {
        result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        results.append(result);
        result.clear();
        continue;
      }

      type = checkExtendedlanguage(subvalue);
      if (type != BCP47Language::NO_EXTENDED_LANGUAGE) {
        result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        results.append(result);
        result.clear();
        continue;
      }

      type = checkScript(subvalue);
      if (type != BCP47Language::NO_SCRIPT) {
        result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        results.append(result);
        result.clear();
        continue;
      }

      type = checkRegion(subvalue);
      if (type != BCP47Language::NO_REGION) {
        result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        results.append(result);
        result.clear();
        continue;
      }

      result->length = subvalue.length();
      result->type = BCP47Language::BAD_SUBTAG;
      results.append(result);
      result.clear();
      continue;
    }
  }

  if (result) {
    results.append(result);
  }

  return results;
}

BCP47Language::TagType
BCP47Languages::checkPrimaryLanguage(const QString& value)
{
  if (value == "i" || value == "x" || (value >= "qaa" && value <= "qtz"))
    return BCP47Language::PRIVATE_LANGUAGE;
  else if (isPrimaryLanguage(value))
    return BCP47Language::PRIMARY_LANGUAGE;
  return BCP47Language::NO_PRIMARY_LANGUAGE;
}

BCP47Language::TagType
BCP47Languages::checkExtendedlanguage(const QString& value)
{
  if (isExtLang(value)) {
    return BCP47Language::EXTENDED_LANGUAGE;
  }
  return BCP47Language::NO_EXTENDED_LANGUAGE;
}

BCP47Language::TagType
BCP47Languages::checkScript(const QString& value)
{
  if (isScript(value)) {
    return BCP47Language::SCRIPT_LANGUAGE;
  } else if (value >= "Qaaa" && value <= "Qabx") {
    return BCP47Language::PRIVATE_SCRIPT;
  }
  return BCP47Language::NO_SCRIPT;
}

BCP47Language::TagType
BCP47Languages::checkRegion(const QString& value)
{
  if (isRegion(value)) {
    return BCP47Language::REGIONAL_LANGUAGE;
  } else if (value == "AA" || (value >= "QM" && value <= "QZ") ||
             (value >= "XA" && value <= "XZ") || value == "ZZ") {
    return BCP47Language::PRIVATE_REGION;
  }
  return BCP47Language::NO_REGION;
}

BCP47Language::TagType
BCP47Languages::checkVariant(const QString& value)
{
  if (isVariant(value)) {
    return BCP47Language::VARIANT_LANGUAGE;
  }
  return BCP47Language::NO_VARIANT_LANGUAGE;
}

BCP47Language::TagType
BCP47Languages::checkGrandfathered(const QString& value)
{
  if (isGrandfathered(value)) {
    return BCP47Language::GRANDFATHERED_LANGUAGE;
  }
  return BCP47Language::NO_GRANDFATHERED_LANGUAGE;
}

BCP47Language::TagType
BCP47Languages::checkRedundant(const QString& value)
{
  if (isVariant(value)) {
    return BCP47Language::REDUNDANT_LANGUAGE;
  }
  return BCP47Language::NO_REDUNDANT_LANGUAGE;
}

bool
BCP47Languages::isPrimaryLanguage(const QString& subtag)
{
  auto keys = m_languageBySubtag.keys();
  return keys.contains(subtag);
}

bool
BCP47Languages::isExtLang(const QString& subtag)
{
  auto keys = m_extlangBySubtag.keys();
  return keys.contains(subtag);
}

bool
BCP47Languages::isVariant(const QString& subtag)
{
  auto keys = m_variantBySubtag.keys();
  return keys.contains(subtag);
}

bool
BCP47Languages::isRegion(const QString& subtag)
{
  auto keys = m_regionBySubtag.keys();
  return keys.contains(subtag);
}

bool
BCP47Languages::isScript(const QString& subtag)
{
  auto keys = m_scriptBySubtag.keys();
  return keys.contains(subtag);
}

bool
BCP47Languages::isGrandfathered(const QString& subtag)
{
  auto keys = m_grandfatheredByTag.keys();
  return keys.contains(subtag);
}

bool
BCP47Languages::isRedundant(const QString& subtag)
{
  auto keys = m_redundantByTag.keys();
  return keys.contains(subtag);
}

QDate
BCP47Languages::fileDate() const
{
  return m_fileDate;
}

QVector<QSharedPointer<BCP47Language>>
BCP47Languages::fromDescription(const QString& description)
{
  return m_datasetByDescription.values(description);
}

QSharedPointer<BCP47Language>
BCP47Languages::languageFromDescription(const QString& description)
{
  return m_languageByDescription.value(description);
}

QSharedPointer<BCP47Language>
BCP47Languages::extlangFromDescription(const QString& description)
{
  return m_extlangByDescription.value(description);
}

QSharedPointer<BCP47Language>
BCP47Languages::variantFromDescription(const QString& description)
{
  return m_variantByDescription.value(description);
}

QSharedPointer<BCP47Language>
BCP47Languages::regionFromDescription(const QString& description)
{
  return m_regionByDescription.value(description);
}

QSharedPointer<BCP47Language>
BCP47Languages::scriptFromDescription(const QString& description)
{
  return m_scriptByDescription.value(description);
}

QSharedPointer<BCP47Language>
BCP47Languages::redundantFromDescription(const QString& description)
{
  return m_redundantByDescription.value(description);
}

QSharedPointer<BCP47Language>
BCP47Languages::grandfatheredFromDescription(const QString& description)
{
  return m_grandfatheredByDescription.value(description);
}

QSharedPointer<BCP47Language>
BCP47Languages::languageFromSubtag(const QString& subtag)
{
  return m_languageBySubtag.value(subtag);
}

QSharedPointer<BCP47Language>
BCP47Languages::extlangFromSubtag(const QString& subtag)
{
  return m_extlangBySubtag.value(subtag);
}

QSharedPointer<BCP47Language>
BCP47Languages::variantFromSubtag(const QString& subtag)
{
  return m_variantBySubtag.value(subtag);
}

QSharedPointer<BCP47Language>
BCP47Languages::regionFromSubtag(const QString& subtag)
{
  return m_regionBySubtag.value(subtag);
}

QSharedPointer<BCP47Language>
BCP47Languages::scriptFromSubtag(const QString& subtag)
{
  return m_scriptBySubtag.value(subtag);
}

QSharedPointer<BCP47Language>
BCP47Languages::redundantFromTag(const QString& tag)
{
  return m_redundantByTag.value(tag);
}

QSharedPointer<BCP47Language>
BCP47Languages::grandfatheredFromTag(const QString& tag)
{
  return m_grandfatheredByTag.value(tag);
}

QVector<QString>
BCP47Languages::extlangsWithPrefix(const QString subtag)
{
  QVector<QString> list;
  auto values = m_extlangBySubtag.values();
  for (auto& extlang : values) {
    if (extlang && extlang->prefix().contains(subtag)) {
      list << extlang->description();
    }
  }
  return list;
}

QVector<QString>
BCP47Languages::variantsWithPrefix(const QString subtag)
{
  QVector<QString> list;
  auto values = m_variantBySubtag.values();
  for (auto& variant : values) {
    if (variant && variant->prefix().contains(subtag)) {
      list << variant->description();
    }
  }
  return list;
}

QVector<QString>
BCP47Languages::descriptions() const
{
  return m_datasetByDescription.keys();
}

QVector<QString>
BCP47Languages::languageDescriptions() const
{
  return m_languageByDescription.keys();
}

QVector<QString>
BCP47Languages::languageSubtags() const
{
  return m_languageBySubtag.keys();
}

QVector<QString>
BCP47Languages::regionDescriptions() const
{
  return m_regionByDescription.keys();
}

QVector<QString>
BCP47Languages::regionSubtags() const
{
  return m_regionBySubtag.keys();
}

QVector<QString>
BCP47Languages::variantDescriptions() const
{
  return m_variantByDescription.keys();
}

QVector<QString>
BCP47Languages::variantSubtags() const
{
  return m_variantBySubtag.keys();
}

QVector<QString>
BCP47Languages::redundantDescriptions() const
{
  return m_redundantByDescription.keys();
}

QVector<QString>
BCP47Languages::redundantTags() const
{
  return m_redundantByTag.keys();
}

QString
BCP47Languages::languageTag(const QString& languageName,
                            const QString& regionName)
{
  auto tag = m_languageBySubtag.value(languageName);
  if (regionName.isEmpty()) {
    if (!tag.isNull())
      return tag->subtag();
  } else {
    auto regTag = m_regionBySubtag.value(regionName);
    if (!tag.isNull() && !regTag.isNull()) {
      return tag->subtag() + "-" + regTag->subtag();
    }
  }
  return QString();
}

QMultiMap<QString, QSharedPointer<BCP47Language>>
BCP47Languages::dataset()
{
  return m_datasetByDescription;
}

QVector<QString>
BCP47Languages::extlangDescriptions() const
{
  return m_extlangByDescription.keys();
}

QVector<QString>
BCP47Languages::extlangSubtags() const
{
  return m_extlangBySubtag.keys();
}

QString
BCP47Languages::extLangTag(const QString& extlanName)
{
  auto langTag = m_languageBySubtag.value(extlanName);
  return langTag->prefix().at(0) + "-" + langTag->preferredValue();
}

void
BCP47Languages::addLanguage(const QString& description,
                            QSharedPointer<BCP47Language> language)
{
  m_datasetByDescription.insert(description, language);
}

//====================================================================
//=== LanguageParser
//====================================================================
LanguageParser::LanguageParser(QObject* parent)
  : QObject(parent)
  , m_lineNumber(0)
{
}

void
LanguageParser::parse()
{
  QString line;
  auto state = BCP47Languages::UNKNOWN;
  bool dateFound = false;
  QSharedPointer<BCP47Language> language;
  int count = 0;
  QMultiMap<QString, QSharedPointer<BCP47Language>> languageMap;
  QMultiMap<int, Errors> errors;
  QDate fileDate;
  QString name, value;

  for (auto& c : m_data) {
    if (c == '\n') {
      m_lineNumber++;
      if (!dateFound) {
        if (line.startsWith("file-date", Qt::CaseInsensitive)) {
          auto splits = line.split(":");
          if (splits.length() == 2) {
            // MUST be trimmed, otherwise pre spaces cause convertion failure.
            auto dateStr = splits.at(1).trimmed();
            auto d = QDate::fromString(dateStr, Qt::ISODate);
            fileDate = d;
            dateFound = true;
            line.clear();
          }
        } else {
          errors.insert(m_lineNumber, BAD_FILE_DATE);
        }
      } else {
        if (line == "%%") { // SEPARATOR for sections
          if (language) {
            for (auto& description : language->descriptions()) {
              languageMap.insert(description, language);
            }
            language = nullptr;
            state = BCP47Languages::FINISHED;
          }
          if (state != BCP47Languages::STARTED) {
            state = BCP47Languages::STARTED;
            language = QSharedPointer<BCP47Language>(new BCP47Language());
          }
          line.clear();
        } else if (line.contains(":")) {
          auto splits = line.split(":", Qt::SkipEmptyParts);
          auto size = splits.size();
          bool nameIsType = false;
          // clear any old values.
          name.clear();
          value.clear();

          if (size == 0) {
            // should never happen
            errors.insert(m_lineNumber, EMPTY_NAME | EMPTY_VALUE);
            continue;
          } else if (size == 1) {
            value = line;
          } else if (size == 2) {
            name = splits.at(0).trimmed();
            nameIsType = BCP47Languages::isType(name);
            value = splits.at(1).trimmed();

            Errors e = NO_ERROR;
            if (state != BCP47Languages::COMMENT) {
              if (!nameIsType) {
                e.setFlag(UNKNOWN_TAG_TYPE, true);
              }
              if (value.isEmpty()) {
                e.setFlag(EMPTY_VALUE, true);
              }
            }
            if (e != NO_ERROR)
              errors.insert(m_lineNumber, EMPTY_NAME);
          } else {
            // If there are more than 1 colon then probably a comment
            // or description so reassemble splits into one value
            name = splits.at(0).trimmed();
            nameIsType = BCP47Languages::isType(name);
            int i;
            if (nameIsType) {
              i = 1;
              value = name;
            } else {
              i = 2;
              value = splits.at(1);
            }
            for (; i < size; i++) {
              value += (":" + splits.at(i));
            }
            // remove excess start/end spaces
            value = value.trimmed();
            size = 2;
          }

          if (size == 2 && nameIsType) {
            auto name = splits.at(0).trimmed().toLower();
            auto lName = name.toLower();
            auto lValue = value.toLower();
            if (lName == "type") {
              if (lValue == "language")
                language->setType(BCP47Language::LANGUAGE);
              else if (lValue == "extlang")
                language->setType(BCP47Language::EXTLANG);
              else if (lValue == "script")
                language->setType(BCP47Language::SCRIPT);
              else if (lValue == "region")
                language->setType(BCP47Language::REGION);
              else if (lValue == "variant")
                language->setType(BCP47Language::VARIANT);
              else if (lValue == "grandfathered")
                language->setType(BCP47Language::GRANDFATHERED);
              else if (lValue == "redundant")
                language->setType(BCP47Language::REDUNDANT);
              else {
                errors.insert(m_lineNumber, UNKNOWN_TAG_TYPE);
              };
              line.clear();
            } else if (lName == "tag") {
              if (language)
                language->setTag(value);
              line.clear();
            } else if (lName == "subtag") {
              if (language)
                language->setSubtag(value);
              line.clear();
            } else if (lName == "description") {
              if (language) {
                language->addDescription(value);
                count++;
                state = BCP47Languages::DESCRIPTION;
              }
              line.clear();
            } else if (lName == "added") {
              if (language)
                language->setDateAdded(QDate::fromString(value, Qt::ISODate));
              line.clear();
            } else if (lName == "suppress-script") {
              if (language)
                language->setSuppressScript(value);
              line.clear();
            } else if (lName == "prefix") {
              if (language)
                language->addPrefix(value);
              line.clear();
            } else if (lName == "macrolanguage") {
              if (language)
                language->setMacrolanguageName(value);
              line.clear();
            } else if (lName == "deprecated") {
              if (language)
                language->setDeprecated(true);
              line.clear();
            } else if (lName == "preferred-value") {
              if (language)
                language->setPreferredValue(value);
              line.clear();
            } else if (lName == "scope") {
              if (value == "macrolanguge") {
                if (language)
                  language->setMacrolanguage(true);
              } else if (value == "collection") {
                if (language)
                  language->setCollection(true);
              }
              line.clear();
            } else if (lName == "comments") {
              if (language) {
                language->setComments(value);
                state = BCP47Languages::COMMENT;
              }
              line.clear();
            } else {
              errors.insert(m_lineNumber, UNKNOWN_TAG_TYPE);
            }
          } else {
            // extra line. Probably a comment or description that runs over.
            switch (state) {
              case BCP47Languages::DESCRIPTION:
                language->appendDescription(line);
                break;
              case BCP47Languages::COMMENT:
                language->appendComment(line);
                break;
              default:
                break;
            }
            line.clear();
          }
        } else {
          // extra line. Probably a comment or description that runs over.
          switch (state) {
            case BCP47Languages::DESCRIPTION:
              language->appendDescription(line);
              break;
            case BCP47Languages::COMMENT:
              language->appendComment(line);
              break;
            default:
              break;
          }
          line.clear();
        }
      }
    } else {
      line += c;
    }
  }
  if (language) {
    for (auto& description : language->descriptions()) {
      languageMap.insert(description, language);
    }
    language = nullptr;
  }
  emit parseCompleted(languageMap, fileDate, errors.isEmpty());
  if (!errors.isEmpty())
    emit parsingErrors(errors);
  emit finished();
}

void
LanguageParser::setData(const QByteArray& data)
{
  m_data = QString(data);
}
