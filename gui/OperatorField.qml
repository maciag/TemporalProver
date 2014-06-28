import QtQuick 2.2
import QtQuick.Controls 1.1

TextField
{
	validator: RegExpValidator
	{
		regExp: /^[^a-z0-9_()\s]*$/;
	}
}