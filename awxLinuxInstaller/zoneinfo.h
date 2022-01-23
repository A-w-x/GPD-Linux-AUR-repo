/*
AwxLinuxInstaller
Copyright (C) A-w-x 2022

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QMap>
#include <QList>

QMap<QString, QList<QString>> zonesList = {
{ "Africa", QList<QString> {"Abidjan","Accra","Addis_Ababa","Algiers","Asmara","Asmera","Bamako","Bangui","Banjul","Bissau","Blantyre","Brazzaville","Bujumbura","Cairo","Casablanca","Ceuta","Conakry","Dakar","Dar_es_Salaam","Djibouti","Douala","El_Aaiun","Freetown","Gaborone","Harare","Johannesburg","Juba","Kampala","Khartoum","Kigali","Kinshasa","Lagos","Libreville","Lome","Luanda","Lubumbashi","Lusaka","Malabo","Maputo","Maseru","Mbabane","Mogadishu","Monrovia","Nairobi","Ndjamena","Niamey","Nouakchott","Ouagadougou","Porto-Novo","Sao_Tome","Timbuktu","Tripoli","Tunis","Windhoek",} },
{ "America", QList<QString> {"Adak","Anchorage","Anguilla","Antigua","Araguaina","Argentina","Aruba","Asuncion","Atikokan","Atka","Bahia","Bahia_Banderas","Barbados","Belem","Belize","Blanc-Sablon","Boa_Vista","Bogota","Boise","Buenos_Aires","Cambridge_Bay","Campo_Grande","Cancun","Caracas","Catamarca","Cayenne","Cayman","Chicago","Chihuahua","Coral_Harbour","Cordoba","Costa_Rica","Creston","Cuiaba","Curacao","Danmarkshavn","Dawson","Dawson_Creek","Denver","Detroit","Dominica","Edmonton","Eirunepe","El_Salvador","Ensenada","Fort_Nelson","Fort_Wayne","Fortaleza","Glace_Bay","Godthab","Goose_Bay","Grand_Turk","Grenada","Guadeloupe","Guatemala","Guayaquil","Guyana","Halifax","Havana","Hermosillo","Indiana","Indianapolis","Inuvik","Iqaluit","Jamaica","Jujuy","Juneau","Kentucky","Knox_IN","Kralendijk","La_Paz","Lima","Los_Angeles","Louisville","Lower_Princes","Maceio","Managua","Manaus","Marigot","Martinique","Matamoros","Mazatlan","Mendoza","Menominee","Merida","Metlakatla","Mexico_City","Miquelon","Moncton","Monterrey","Montevideo","Montreal","Montserrat","Nassau","New_York","Nipigon","Nome","Noronha","North_Dakota","Nuuk","Ojinaga","Panama","Pangnirtung","Paramaribo","Phoenix","Port-au-Prince","Port_of_Spain","Porto_Acre","Porto_Velho","Puerto_Rico","Punta_Arenas","Rainy_River","Rankin_Inlet","Recife","Regina","Resolute","Rio_Branco","Rosario","Santa_Isabel","Santarem","Santiago","Santo_Domingo","Sao_Paulo","Scoresbysund","Shiprock","Sitka","St_Barthelemy","St_Johns","St_Kitts","St_Lucia","St_Thomas","St_Vincent","Swift_Current","Tegucigalpa","Thule","Thunder_Bay","Tijuana","Toronto","Tortola","Vancouver","Virgin","Whitehorse","Winnipeg","Yakutat","Yellowknife",} },
{ "Antarctica", QList<QString> {"Casey","Davis","DumontDUrville","Macquarie","Mawson","McMurdo","Palmer","Rothera","South_Pole","Syowa","Troll","Vostok",} },
{ "Arctic", QList<QString> {"Longyearbyen",} },
{ "Asia", QList<QString> {"Aden","Almaty","Amman","Anadyr","Aqtau","Aqtobe","Ashgabat","Ashkhabad","Atyrau","Baghdad","Bahrain","Baku","Bangkok","Barnaul","Beirut","Bishkek","Brunei","Calcutta","Chita","Choibalsan","Chongqing","Chungking","Colombo","Dacca","Damascus","Dhaka","Dili","Dubai","Dushanbe","Famagusta","Gaza","Harbin","Hebron","Ho_Chi_Minh","Hong_Kong","Hovd","Irkutsk","Istanbul","Jakarta","Jayapura","Jerusalem","Kabul","Kamchatka","Karachi","Kashgar","Kathmandu","Katmandu","Khandyga","Kolkata","Krasnoyarsk","Kuala_Lumpur","Kuching","Kuwait","Macao","Macau","Magadan","Makassar","Manila","Muscat","Nicosia","Novokuznetsk","Novosibirsk","Omsk","Oral","Phnom_Penh","Pontianak","Pyongyang","Qatar","Qostanay","Qyzylorda","Rangoon","Riyadh","Saigon","Sakhalin","Samarkand","Seoul","Shanghai","Singapore","Srednekolymsk","Taipei","Tashkent","Tbilisi","Tehran","Tel_Aviv","Thimbu","Thimphu","Tokyo","Tomsk","Ujung_Pandang","Ulaanbaatar","Ulan_Bator","Urumqi","Ust-Nera","Vientiane","Vladivostok","Yakutsk","Yangon","Yekaterinburg","Yerevan",} },
{ "Atlantic", QList<QString> {"Azores","Bermuda","Canary","Cape_Verde","Faeroe","Faroe","Jan_Mayen","Madeira","Reykjavik","South_Georgia","St_Helena","Stanley",} },
{ "Australia", QList<QString> {"ACT","Adelaide","Brisbane","Broken_Hill","Canberra","Currie","Darwin","Eucla","Hobart","LHI","Lindeman","Lord_Howe","Melbourne","NSW","North","Perth","Queensland","South","Sydney","Tasmania","Victoria","West","Yancowinna",} },
{ "Brazil", QList<QString> {"Acre","DeNoronha","East","West",} },
{ "Canada", QList<QString> {"Atlantic","Central","Eastern","Mountain","Newfoundland","Pacific","Saskatchewan","Yukon",} },
{ "Chile", QList<QString> {"Continental","EasterIsland",} },
{ "Cuba", QList<QString>() },
{ "Egypt", QList<QString>() },
{ "Eire", QList<QString>() },
{ "Europe", QList<QString> {"Amsterdam","Andorra","Astrakhan","Athens","Belfast","Belgrade","Berlin","Bratislava","Brussels","Bucharest","Budapest","Busingen","Chisinau","Copenhagen","Dublin","Gibraltar","Guernsey","Helsinki","Isle_of_Man","Istanbul","Jersey","Kaliningrad","Kiev","Kirov","Lisbon","Ljubljana","London","Luxembourg","Madrid","Malta","Mariehamn","Minsk","Monaco","Moscow","Nicosia","Oslo","Paris","Podgorica","Prague","Riga","Rome","Samara","San_Marino","Sarajevo","Saratov","Simferopol","Skopje","Sofia","Stockholm","Tallinn","Tirane","Tiraspol","Ulyanovsk","Uzhgorod","Vaduz","Vatican","Vienna","Vilnius","Volgograd","Warsaw","Zagreb","Zaporozhye","Zurich",} },
{ "GB", QList<QString>() },
{ "Hongkong", QList<QString>() },
{ "Iceland", QList<QString>() },
{ "Indian", QList<QString> {"Antananarivo","Chagos","Christmas","Cocos","Comoro","Kerguelen","Mahe","Maldives","Mauritius","Mayotte","Reunion",} },
{ "Iran", QList<QString>() },
{ "Israel", QList<QString>() },
{ "Jamaica", QList<QString>() },
{ "Japan", QList<QString>() },
{ "Libya", QList<QString>() },
{ "Mexico", QList<QString> {"BajaNorte","BajaSur","General",} },
{ "Navajo", QList<QString>() },
{ "Pacific", QList<QString> {"Apia","Auckland","Bougainville","Chatham","Chuuk","Easter","Efate","Enderbury","Fakaofo","Fiji","Funafuti","Galapagos","Gambier","Guadalcanal","Guam","Honolulu","Johnston","Kanton","Kiritimati","Kosrae","Kwajalein","Majuro","Marquesas","Midway","Nauru","Niue","Norfolk","Noumea","Pago_Pago","Palau","Pitcairn","Pohnpei","Ponape","Port_Moresby","Rarotonga","Saipan","Samoa","Tahiti","Tarawa","Tongatapu","Truk","Wake","Wallis","Yap",} },
{ "Poland", QList<QString>() },
{ "Portugal", QList<QString>() },
{ "Singapore", QList<QString>() },
{ "Turkey", QList<QString>() },
};
