# KTNEF - an API for handling TNEF data

## Purpose

The ktnef library contains an API for the handling of TNEF data.

The API permits access to the actual attachments, the message
properties (TNEF/MAPI), and allows one to view/extract message formatted
text in Rich Text Format format.

## Description

To quote [Wikipedia](https://en.wikipedia.org/wiki/TNEF):

> *Transport Neutral Encapsulation Format* or TNEF is a proprietary format of
> e-mail attachment used by Microsoft Outlook and Microsoft Exchange Server.
> An attached file with TNEF encoding is most usually called winmail.dat or
> win.dat.
>
> Within the Outlook email client TNEF encoding cannot be explicitly enabled or
> disabled.  Selecting <a href="https://en.wikipedia.org/wiki/Rich_Text_Format">
> RTF</a> (Rich Text Format) as the format for sending an e-mail implicitly
> enables TNEF encoding, using it in preference to the more common and widely
> compatible MIME standard.  When sending plain-text or HTML format messages,
> Outlook uses MIME.
>
> Some TNEF files only contain information used by Outlook to generate a richly
> formatted view of the message, embedded (OLE) documents or Outlook-specific
> features such as forms, voting buttons, and meeting requests.  Other TNEF
> files may contain files which have been attached to an e-mail message.

## Authors

The major authors of this library are:\n
Michael Goffioul \<kdeprint@swing.be\>,
Bo Thorsen \<bo@sonofthor.dk\>

## Maintainers

Michael Goffioul \<goffioul@imec.be\>,
Allen Winter \<winter@kde.org\>

## License

This library is licensed under the [LGPL-2.0-or-later](https://invent.kde.org/pim/ktnef/-/blob/master/LICENSES/LGPL-2.0-or-later.txt?ref_type=heads)
