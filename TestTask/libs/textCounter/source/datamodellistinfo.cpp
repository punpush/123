#include "datamodellistinfo.h"

DataModelListInfo::DataModelListInfo()
{

}

void DataModelListInfo::setLetters(int value)
{
    m_letters = value;
}

int DataModelListInfo::letters() const
{
    return m_letters;
}

void DataModelListInfo::setWords(int value)
{
    m_words = value;
}

int DataModelListInfo::words() const
{
    return m_words;
}

void DataModelListInfo::setSentences(int value)
{
    m_sentences = value;
}

int DataModelListInfo::sentences() const
{
    return m_sentences;
}
