#include "Emoticons.h"
Emoticons::Emoticons()
{
}
void Emoticons::replaceEmoticon(QString *message)
{
    message->replace(":)","🙂");
    message->replace(";)","🙂");
    message->replace(":P","😜");
    message->replace(";P","😜");
    message->replace(":*","😘");
    message->replace(";*","😘");
    message->replace("xD","😆");
    message->replace(";)","😉");
    message->replace("8)","😎");
    message->replace(":(","🙁");
    message->replace(";(","🙁");
    message->replace(":D","😀");
    message->replace(";D","😀");
}
