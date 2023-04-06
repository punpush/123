#ifndef DATAMODELLISTINFO_H
#define DATAMODELLISTINFO_H


class DataModelListInfo
{
public:
    DataModelListInfo();

    void setLetters(int value);
    int letters() const;

    void setWords(int value);
    int words() const;

    void setSentences(int value);
    int sentences() const;

private:
    int m_letters;
    int m_words;
    int m_sentences;
};

#endif // DATAMODELLISTINFO_H
