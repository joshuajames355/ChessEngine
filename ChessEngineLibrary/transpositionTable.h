#pragma once
#include <cstdlib> 
#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "transpositionTable.h"

#define TTSize 16777216

struct TranspositionEntry;

struct ZorbistKeys
{
	//A key for each square. WP = 0 
public:
	static uint64_t pieceKeys[64][12];
	static uint64_t enPassantKeys[8];
	static uint64_t whiteMoveKey;
	static uint64_t blackQueenSideCastlingKey;
	static uint64_t blackKingSideCastlingKey;
	static uint64_t whiteQueenSideCastlingKey;
	static uint64_t whiteKingSideCastlingKey;
	
	static void initialize();
};

uint64_t get64rand();

const uint64_t PolygotPieceKeys[781] = {
	uint64_t(0x9D39247E33776D41), uint64_t(0x2AF7398005AAA5C7), uint64_t(0x44DB015024623547), uint64_t(0x9C15F73E62A76AE2),
	uint64_t(0x75834465489C0C89), uint64_t(0x3290AC3A203001BF), uint64_t(0x0FBBAD1F61042279), uint64_t(0xE83A908FF2FB60CA),
	uint64_t(0x0D7E765D58755C10), uint64_t(0x1A083822CEAFE02D), uint64_t(0x9605D5F0E25EC3B0), uint64_t(0xD021FF5CD13A2ED5),
	uint64_t(0x40BDF15D4A672E32), uint64_t(0x011355146FD56395), uint64_t(0x5DB4832046F3D9E5), uint64_t(0x239F8B2D7FF719CC),
	uint64_t(0x05D1A1AE85B49AA1), uint64_t(0x679F848F6E8FC971), uint64_t(0x7449BBFF801FED0B), uint64_t(0x7D11CDB1C3B7ADF0),
	uint64_t(0x82C7709E781EB7CC), uint64_t(0xF3218F1C9510786C), uint64_t(0x331478F3AF51BBE6), uint64_t(0x4BB38DE5E7219443),
	uint64_t(0xAA649C6EBCFD50FC), uint64_t(0x8DBD98A352AFD40B), uint64_t(0x87D2074B81D79217), uint64_t(0x19F3C751D3E92AE1),
	uint64_t(0xB4AB30F062B19ABF), uint64_t(0x7B0500AC42047AC4), uint64_t(0xC9452CA81A09D85D), uint64_t(0x24AA6C514DA27500),
	uint64_t(0x4C9F34427501B447), uint64_t(0x14A68FD73C910841), uint64_t(0xA71B9B83461CBD93), uint64_t(0x03488B95B0F1850F),
	uint64_t(0x637B2B34FF93C040), uint64_t(0x09D1BC9A3DD90A94), uint64_t(0x3575668334A1DD3B), uint64_t(0x735E2B97A4C45A23),
	uint64_t(0x18727070F1BD400B), uint64_t(0x1FCBACD259BF02E7), uint64_t(0xD310A7C2CE9B6555), uint64_t(0xBF983FE0FE5D8244),
	uint64_t(0x9F74D14F7454A824), uint64_t(0x51EBDC4AB9BA3035), uint64_t(0x5C82C505DB9AB0FA), uint64_t(0xFCF7FE8A3430B241),
	uint64_t(0x3253A729B9BA3DDE), uint64_t(0x8C74C368081B3075), uint64_t(0xB9BC6C87167C33E7), uint64_t(0x7EF48F2B83024E20),
	uint64_t(0x11D505D4C351BD7F), uint64_t(0x6568FCA92C76A243), uint64_t(0x4DE0B0F40F32A7B8), uint64_t(0x96D693460CC37E5D),
	uint64_t(0x42E240CB63689F2F), uint64_t(0x6D2BDCDAE2919661), uint64_t(0x42880B0236E4D951), uint64_t(0x5F0F4A5898171BB6),
	uint64_t(0x39F890F579F92F88), uint64_t(0x93C5B5F47356388B), uint64_t(0x63DC359D8D231B78), uint64_t(0xEC16CA8AEA98AD76),
	uint64_t(0x5355F900C2A82DC7), uint64_t(0x07FB9F855A997142), uint64_t(0x5093417AA8A7ED5E), uint64_t(0x7BCBC38DA25A7F3C),
	uint64_t(0x19FC8A768CF4B6D4), uint64_t(0x637A7780DECFC0D9), uint64_t(0x8249A47AEE0E41F7), uint64_t(0x79AD695501E7D1E8),
	uint64_t(0x14ACBAF4777D5776), uint64_t(0xF145B6BECCDEA195), uint64_t(0xDABF2AC8201752FC), uint64_t(0x24C3C94DF9C8D3F6),
	uint64_t(0xBB6E2924F03912EA), uint64_t(0x0CE26C0B95C980D9), uint64_t(0xA49CD132BFBF7CC4), uint64_t(0xE99D662AF4243939),
	uint64_t(0x27E6AD7891165C3F), uint64_t(0x8535F040B9744FF1), uint64_t(0x54B3F4FA5F40D873), uint64_t(0x72B12C32127FED2B),
	uint64_t(0xEE954D3C7B411F47), uint64_t(0x9A85AC909A24EAA1), uint64_t(0x70AC4CD9F04F21F5), uint64_t(0xF9B89D3E99A075C2),
	uint64_t(0x87B3E2B2B5C907B1), uint64_t(0xA366E5B8C54F48B8), uint64_t(0xAE4A9346CC3F7CF2), uint64_t(0x1920C04D47267BBD),
	uint64_t(0x87BF02C6B49E2AE9), uint64_t(0x092237AC237F3859), uint64_t(0xFF07F64EF8ED14D0), uint64_t(0x8DE8DCA9F03CC54E),
	uint64_t(0x9C1633264DB49C89), uint64_t(0xB3F22C3D0B0B38ED), uint64_t(0x390E5FB44D01144B), uint64_t(0x5BFEA5B4712768E9),
	uint64_t(0x1E1032911FA78984), uint64_t(0x9A74ACB964E78CB3), uint64_t(0x4F80F7A035DAFB04), uint64_t(0x6304D09A0B3738C4),
	uint64_t(0x2171E64683023A08), uint64_t(0x5B9B63EB9CEFF80C), uint64_t(0x506AACF489889342), uint64_t(0x1881AFC9A3A701D6),
	uint64_t(0x6503080440750644), uint64_t(0xDFD395339CDBF4A7), uint64_t(0xEF927DBCF00C20F2), uint64_t(0x7B32F7D1E03680EC),
	uint64_t(0xB9FD7620E7316243), uint64_t(0x05A7E8A57DB91B77), uint64_t(0xB5889C6E15630A75), uint64_t(0x4A750A09CE9573F7),
	uint64_t(0xCF464CEC899A2F8A), uint64_t(0xF538639CE705B824), uint64_t(0x3C79A0FF5580EF7F), uint64_t(0xEDE6C87F8477609D),
	uint64_t(0x799E81F05BC93F31), uint64_t(0x86536B8CF3428A8C), uint64_t(0x97D7374C60087B73), uint64_t(0xA246637CFF328532),
	uint64_t(0x043FCAE60CC0EBA0), uint64_t(0x920E449535DD359E), uint64_t(0x70EB093B15B290CC), uint64_t(0x73A1921916591CBD),
	uint64_t(0x56436C9FE1A1AA8D), uint64_t(0xEFAC4B70633B8F81), uint64_t(0xBB215798D45DF7AF), uint64_t(0x45F20042F24F1768),
	uint64_t(0x930F80F4E8EB7462), uint64_t(0xFF6712FFCFD75EA1), uint64_t(0xAE623FD67468AA70), uint64_t(0xDD2C5BC84BC8D8FC),
	uint64_t(0x7EED120D54CF2DD9), uint64_t(0x22FE545401165F1C), uint64_t(0xC91800E98FB99929), uint64_t(0x808BD68E6AC10365),
	uint64_t(0xDEC468145B7605F6), uint64_t(0x1BEDE3A3AEF53302), uint64_t(0x43539603D6C55602), uint64_t(0xAA969B5C691CCB7A),
	uint64_t(0xA87832D392EFEE56), uint64_t(0x65942C7B3C7E11AE), uint64_t(0xDED2D633CAD004F6), uint64_t(0x21F08570F420E565),
	uint64_t(0xB415938D7DA94E3C), uint64_t(0x91B859E59ECB6350), uint64_t(0x10CFF333E0ED804A), uint64_t(0x28AED140BE0BB7DD),
	uint64_t(0xC5CC1D89724FA456), uint64_t(0x5648F680F11A2741), uint64_t(0x2D255069F0B7DAB3), uint64_t(0x9BC5A38EF729ABD4),
	uint64_t(0xEF2F054308F6A2BC), uint64_t(0xAF2042F5CC5C2858), uint64_t(0x480412BAB7F5BE2A), uint64_t(0xAEF3AF4A563DFE43),
	uint64_t(0x19AFE59AE451497F), uint64_t(0x52593803DFF1E840), uint64_t(0xF4F076E65F2CE6F0), uint64_t(0x11379625747D5AF3),
	uint64_t(0xBCE5D2248682C115), uint64_t(0x9DA4243DE836994F), uint64_t(0x066F70B33FE09017), uint64_t(0x4DC4DE189B671A1C),
	uint64_t(0x51039AB7712457C3), uint64_t(0xC07A3F80C31FB4B4), uint64_t(0xB46EE9C5E64A6E7C), uint64_t(0xB3819A42ABE61C87),
	uint64_t(0x21A007933A522A20), uint64_t(0x2DF16F761598AA4F), uint64_t(0x763C4A1371B368FD), uint64_t(0xF793C46702E086A0),
	uint64_t(0xD7288E012AEB8D31), uint64_t(0xDE336A2A4BC1C44B), uint64_t(0x0BF692B38D079F23), uint64_t(0x2C604A7A177326B3),
	uint64_t(0x4850E73E03EB6064), uint64_t(0xCFC447F1E53C8E1B), uint64_t(0xB05CA3F564268D99), uint64_t(0x9AE182C8BC9474E8),
	uint64_t(0xA4FC4BD4FC5558CA), uint64_t(0xE755178D58FC4E76), uint64_t(0x69B97DB1A4C03DFE), uint64_t(0xF9B5B7C4ACC67C96),
	uint64_t(0xFC6A82D64B8655FB), uint64_t(0x9C684CB6C4D24417), uint64_t(0x8EC97D2917456ED0), uint64_t(0x6703DF9D2924E97E),
	uint64_t(0xC547F57E42A7444E), uint64_t(0x78E37644E7CAD29E), uint64_t(0xFE9A44E9362F05FA), uint64_t(0x08BD35CC38336615),
	uint64_t(0x9315E5EB3A129ACE), uint64_t(0x94061B871E04DF75), uint64_t(0xDF1D9F9D784BA010), uint64_t(0x3BBA57B68871B59D),
	uint64_t(0xD2B7ADEEDED1F73F), uint64_t(0xF7A255D83BC373F8), uint64_t(0xD7F4F2448C0CEB81), uint64_t(0xD95BE88CD210FFA7),
	uint64_t(0x336F52F8FF4728E7), uint64_t(0xA74049DAC312AC71), uint64_t(0xA2F61BB6E437FDB5), uint64_t(0x4F2A5CB07F6A35B3),
	uint64_t(0x87D380BDA5BF7859), uint64_t(0x16B9F7E06C453A21), uint64_t(0x7BA2484C8A0FD54E), uint64_t(0xF3A678CAD9A2E38C),
	uint64_t(0x39B0BF7DDE437BA2), uint64_t(0xFCAF55C1BF8A4424), uint64_t(0x18FCF680573FA594), uint64_t(0x4C0563B89F495AC3),
	uint64_t(0x40E087931A00930D), uint64_t(0x8CFFA9412EB642C1), uint64_t(0x68CA39053261169F), uint64_t(0x7A1EE967D27579E2),
	uint64_t(0x9D1D60E5076F5B6F), uint64_t(0x3810E399B6F65BA2), uint64_t(0x32095B6D4AB5F9B1), uint64_t(0x35CAB62109DD038A),
	uint64_t(0xA90B24499FCFAFB1), uint64_t(0x77A225A07CC2C6BD), uint64_t(0x513E5E634C70E331), uint64_t(0x4361C0CA3F692F12),
	uint64_t(0xD941ACA44B20A45B), uint64_t(0x528F7C8602C5807B), uint64_t(0x52AB92BEB9613989), uint64_t(0x9D1DFA2EFC557F73),
	uint64_t(0x722FF175F572C348), uint64_t(0x1D1260A51107FE97), uint64_t(0x7A249A57EC0C9BA2), uint64_t(0x04208FE9E8F7F2D6),
	uint64_t(0x5A110C6058B920A0), uint64_t(0x0CD9A497658A5698), uint64_t(0x56FD23C8F9715A4C), uint64_t(0x284C847B9D887AAE),
	uint64_t(0x04FEABFBBDB619CB), uint64_t(0x742E1E651C60BA83), uint64_t(0x9A9632E65904AD3C), uint64_t(0x881B82A13B51B9E2),
	uint64_t(0x506E6744CD974924), uint64_t(0xB0183DB56FFC6A79), uint64_t(0x0ED9B915C66ED37E), uint64_t(0x5E11E86D5873D484),
	uint64_t(0xF678647E3519AC6E), uint64_t(0x1B85D488D0F20CC5), uint64_t(0xDAB9FE6525D89021), uint64_t(0x0D151D86ADB73615),
	uint64_t(0xA865A54EDCC0F019), uint64_t(0x93C42566AEF98FFB), uint64_t(0x99E7AFEABE000731), uint64_t(0x48CBFF086DDF285A),
	uint64_t(0x7F9B6AF1EBF78BAF), uint64_t(0x58627E1A149BBA21), uint64_t(0x2CD16E2ABD791E33), uint64_t(0xD363EFF5F0977996),
	uint64_t(0x0CE2A38C344A6EED), uint64_t(0x1A804AADB9CFA741), uint64_t(0x907F30421D78C5DE), uint64_t(0x501F65EDB3034D07),
	uint64_t(0x37624AE5A48FA6E9), uint64_t(0x957BAF61700CFF4E), uint64_t(0x3A6C27934E31188A), uint64_t(0xD49503536ABCA345),
	uint64_t(0x088E049589C432E0), uint64_t(0xF943AEE7FEBF21B8), uint64_t(0x6C3B8E3E336139D3), uint64_t(0x364F6FFA464EE52E),
	uint64_t(0xD60F6DCEDC314222), uint64_t(0x56963B0DCA418FC0), uint64_t(0x16F50EDF91E513AF), uint64_t(0xEF1955914B609F93),
	uint64_t(0x565601C0364E3228), uint64_t(0xECB53939887E8175), uint64_t(0xBAC7A9A18531294B), uint64_t(0xB344C470397BBA52),
	uint64_t(0x65D34954DAF3CEBD), uint64_t(0xB4B81B3FA97511E2), uint64_t(0xB422061193D6F6A7), uint64_t(0x071582401C38434D),
	uint64_t(0x7A13F18BBEDC4FF5), uint64_t(0xBC4097B116C524D2), uint64_t(0x59B97885E2F2EA28), uint64_t(0x99170A5DC3115544),
	uint64_t(0x6F423357E7C6A9F9), uint64_t(0x325928EE6E6F8794), uint64_t(0xD0E4366228B03343), uint64_t(0x565C31F7DE89EA27),
	uint64_t(0x30F5611484119414), uint64_t(0xD873DB391292ED4F), uint64_t(0x7BD94E1D8E17DEBC), uint64_t(0xC7D9F16864A76E94),
	uint64_t(0x947AE053EE56E63C), uint64_t(0xC8C93882F9475F5F), uint64_t(0x3A9BF55BA91F81CA), uint64_t(0xD9A11FBB3D9808E4),
	uint64_t(0x0FD22063EDC29FCA), uint64_t(0xB3F256D8ACA0B0B9), uint64_t(0xB03031A8B4516E84), uint64_t(0x35DD37D5871448AF),
	uint64_t(0xE9F6082B05542E4E), uint64_t(0xEBFAFA33D7254B59), uint64_t(0x9255ABB50D532280), uint64_t(0xB9AB4CE57F2D34F3),
	uint64_t(0x693501D628297551), uint64_t(0xC62C58F97DD949BF), uint64_t(0xCD454F8F19C5126A), uint64_t(0xBBE83F4ECC2BDECB),
	uint64_t(0xDC842B7E2819E230), uint64_t(0xBA89142E007503B8), uint64_t(0xA3BC941D0A5061CB), uint64_t(0xE9F6760E32CD8021),
	uint64_t(0x09C7E552BC76492F), uint64_t(0x852F54934DA55CC9), uint64_t(0x8107FCCF064FCF56), uint64_t(0x098954D51FFF6580),
	uint64_t(0x23B70EDB1955C4BF), uint64_t(0xC330DE426430F69D), uint64_t(0x4715ED43E8A45C0A), uint64_t(0xA8D7E4DAB780A08D),
	uint64_t(0x0572B974F03CE0BB), uint64_t(0xB57D2E985E1419C7), uint64_t(0xE8D9ECBE2CF3D73F), uint64_t(0x2FE4B17170E59750),
	uint64_t(0x11317BA87905E790), uint64_t(0x7FBF21EC8A1F45EC), uint64_t(0x1725CABFCB045B00), uint64_t(0x964E915CD5E2B207),
	uint64_t(0x3E2B8BCBF016D66D), uint64_t(0xBE7444E39328A0AC), uint64_t(0xF85B2B4FBCDE44B7), uint64_t(0x49353FEA39BA63B1),
	uint64_t(0x1DD01AAFCD53486A), uint64_t(0x1FCA8A92FD719F85), uint64_t(0xFC7C95D827357AFA), uint64_t(0x18A6A990C8B35EBD),
	uint64_t(0xCCCB7005C6B9C28D), uint64_t(0x3BDBB92C43B17F26), uint64_t(0xAA70B5B4F89695A2), uint64_t(0xE94C39A54A98307F),
	uint64_t(0xB7A0B174CFF6F36E), uint64_t(0xD4DBA84729AF48AD), uint64_t(0x2E18BC1AD9704A68), uint64_t(0x2DE0966DAF2F8B1C),
	uint64_t(0xB9C11D5B1E43A07E), uint64_t(0x64972D68DEE33360), uint64_t(0x94628D38D0C20584), uint64_t(0xDBC0D2B6AB90A559),
	uint64_t(0xD2733C4335C6A72F), uint64_t(0x7E75D99D94A70F4D), uint64_t(0x6CED1983376FA72B), uint64_t(0x97FCAACBF030BC24),
	uint64_t(0x7B77497B32503B12), uint64_t(0x8547EDDFB81CCB94), uint64_t(0x79999CDFF70902CB), uint64_t(0xCFFE1939438E9B24),
	uint64_t(0x829626E3892D95D7), uint64_t(0x92FAE24291F2B3F1), uint64_t(0x63E22C147B9C3403), uint64_t(0xC678B6D860284A1C),
	uint64_t(0x5873888850659AE7), uint64_t(0x0981DCD296A8736D), uint64_t(0x9F65789A6509A440), uint64_t(0x9FF38FED72E9052F),
	uint64_t(0xE479EE5B9930578C), uint64_t(0xE7F28ECD2D49EECD), uint64_t(0x56C074A581EA17FE), uint64_t(0x5544F7D774B14AEF),
	uint64_t(0x7B3F0195FC6F290F), uint64_t(0x12153635B2C0CF57), uint64_t(0x7F5126DBBA5E0CA7), uint64_t(0x7A76956C3EAFB413),
	uint64_t(0x3D5774A11D31AB39), uint64_t(0x8A1B083821F40CB4), uint64_t(0x7B4A38E32537DF62), uint64_t(0x950113646D1D6E03),
	uint64_t(0x4DA8979A0041E8A9), uint64_t(0x3BC36E078F7515D7), uint64_t(0x5D0A12F27AD310D1), uint64_t(0x7F9D1A2E1EBE1327),
	uint64_t(0xDA3A361B1C5157B1), uint64_t(0xDCDD7D20903D0C25), uint64_t(0x36833336D068F707), uint64_t(0xCE68341F79893389),
	uint64_t(0xAB9090168DD05F34), uint64_t(0x43954B3252DC25E5), uint64_t(0xB438C2B67F98E5E9), uint64_t(0x10DCD78E3851A492),
	uint64_t(0xDBC27AB5447822BF), uint64_t(0x9B3CDB65F82CA382), uint64_t(0xB67B7896167B4C84), uint64_t(0xBFCED1B0048EAC50),
	uint64_t(0xA9119B60369FFEBD), uint64_t(0x1FFF7AC80904BF45), uint64_t(0xAC12FB171817EEE7), uint64_t(0xAF08DA9177DDA93D),
	uint64_t(0x1B0CAB936E65C744), uint64_t(0xB559EB1D04E5E932), uint64_t(0xC37B45B3F8D6F2BA), uint64_t(0xC3A9DC228CAAC9E9),
	uint64_t(0xF3B8B6675A6507FF), uint64_t(0x9FC477DE4ED681DA), uint64_t(0x67378D8ECCEF96CB), uint64_t(0x6DD856D94D259236),
	uint64_t(0xA319CE15B0B4DB31), uint64_t(0x073973751F12DD5E), uint64_t(0x8A8E849EB32781A5), uint64_t(0xE1925C71285279F5),
	uint64_t(0x74C04BF1790C0EFE), uint64_t(0x4DDA48153C94938A), uint64_t(0x9D266D6A1CC0542C), uint64_t(0x7440FB816508C4FE),
	uint64_t(0x13328503DF48229F), uint64_t(0xD6BF7BAEE43CAC40), uint64_t(0x4838D65F6EF6748F), uint64_t(0x1E152328F3318DEA),
	uint64_t(0x8F8419A348F296BF), uint64_t(0x72C8834A5957B511), uint64_t(0xD7A023A73260B45C), uint64_t(0x94EBC8ABCFB56DAE),
	uint64_t(0x9FC10D0F989993E0), uint64_t(0xDE68A2355B93CAE6), uint64_t(0xA44CFE79AE538BBE), uint64_t(0x9D1D84FCCE371425),
	uint64_t(0x51D2B1AB2DDFB636), uint64_t(0x2FD7E4B9E72CD38C), uint64_t(0x65CA5B96B7552210), uint64_t(0xDD69A0D8AB3B546D),
	uint64_t(0x604D51B25FBF70E2), uint64_t(0x73AA8A564FB7AC9E), uint64_t(0x1A8C1E992B941148), uint64_t(0xAAC40A2703D9BEA0),
	uint64_t(0x764DBEAE7FA4F3A6), uint64_t(0x1E99B96E70A9BE8B), uint64_t(0x2C5E9DEB57EF4743), uint64_t(0x3A938FEE32D29981),
	uint64_t(0x26E6DB8FFDF5ADFE), uint64_t(0x469356C504EC9F9D), uint64_t(0xC8763C5B08D1908C), uint64_t(0x3F6C6AF859D80055),
	uint64_t(0x7F7CC39420A3A545), uint64_t(0x9BFB227EBDF4C5CE), uint64_t(0x89039D79D6FC5C5C), uint64_t(0x8FE88B57305E2AB6),
	uint64_t(0xA09E8C8C35AB96DE), uint64_t(0xFA7E393983325753), uint64_t(0xD6B6D0ECC617C699), uint64_t(0xDFEA21EA9E7557E3),
	uint64_t(0xB67C1FA481680AF8), uint64_t(0xCA1E3785A9E724E5), uint64_t(0x1CFC8BED0D681639), uint64_t(0xD18D8549D140CAEA),
	uint64_t(0x4ED0FE7E9DC91335), uint64_t(0xE4DBF0634473F5D2), uint64_t(0x1761F93A44D5AEFE), uint64_t(0x53898E4C3910DA55),
	uint64_t(0x734DE8181F6EC39A), uint64_t(0x2680B122BAA28D97), uint64_t(0x298AF231C85BAFAB), uint64_t(0x7983EED3740847D5),
	uint64_t(0x66C1A2A1A60CD889), uint64_t(0x9E17E49642A3E4C1), uint64_t(0xEDB454E7BADC0805), uint64_t(0x50B704CAB602C329),
	uint64_t(0x4CC317FB9CDDD023), uint64_t(0x66B4835D9EAFEA22), uint64_t(0x219B97E26FFC81BD), uint64_t(0x261E4E4C0A333A9D),
	uint64_t(0x1FE2CCA76517DB90), uint64_t(0xD7504DFA8816EDBB), uint64_t(0xB9571FA04DC089C8), uint64_t(0x1DDC0325259B27DE),
	uint64_t(0xCF3F4688801EB9AA), uint64_t(0xF4F5D05C10CAB243), uint64_t(0x38B6525C21A42B0E), uint64_t(0x36F60E2BA4FA6800),
	uint64_t(0xEB3593803173E0CE), uint64_t(0x9C4CD6257C5A3603), uint64_t(0xAF0C317D32ADAA8A), uint64_t(0x258E5A80C7204C4B),
	uint64_t(0x8B889D624D44885D), uint64_t(0xF4D14597E660F855), uint64_t(0xD4347F66EC8941C3), uint64_t(0xE699ED85B0DFB40D),
	uint64_t(0x2472F6207C2D0484), uint64_t(0xC2A1E7B5B459AEB5), uint64_t(0xAB4F6451CC1D45EC), uint64_t(0x63767572AE3D6174),
	uint64_t(0xA59E0BD101731A28), uint64_t(0x116D0016CB948F09), uint64_t(0x2CF9C8CA052F6E9F), uint64_t(0x0B090A7560A968E3),
	uint64_t(0xABEEDDB2DDE06FF1), uint64_t(0x58EFC10B06A2068D), uint64_t(0xC6E57A78FBD986E0), uint64_t(0x2EAB8CA63CE802D7),
	uint64_t(0x14A195640116F336), uint64_t(0x7C0828DD624EC390), uint64_t(0xD74BBE77E6116AC7), uint64_t(0x804456AF10F5FB53),
	uint64_t(0xEBE9EA2ADF4321C7), uint64_t(0x03219A39EE587A30), uint64_t(0x49787FEF17AF9924), uint64_t(0xA1E9300CD8520548),
	uint64_t(0x5B45E522E4B1B4EF), uint64_t(0xB49C3B3995091A36), uint64_t(0xD4490AD526F14431), uint64_t(0x12A8F216AF9418C2),
	uint64_t(0x001F837CC7350524), uint64_t(0x1877B51E57A764D5), uint64_t(0xA2853B80F17F58EE), uint64_t(0x993E1DE72D36D310),
	uint64_t(0xB3598080CE64A656), uint64_t(0x252F59CF0D9F04BB), uint64_t(0xD23C8E176D113600), uint64_t(0x1BDA0492E7E4586E),
	uint64_t(0x21E0BD5026C619BF), uint64_t(0x3B097ADAF088F94E), uint64_t(0x8D14DEDB30BE846E), uint64_t(0xF95CFFA23AF5F6F4),
	uint64_t(0x3871700761B3F743), uint64_t(0xCA672B91E9E4FA16), uint64_t(0x64C8E531BFF53B55), uint64_t(0x241260ED4AD1E87D),
	uint64_t(0x106C09B972D2E822), uint64_t(0x7FBA195410E5CA30), uint64_t(0x7884D9BC6CB569D8), uint64_t(0x0647DFEDCD894A29),
	uint64_t(0x63573FF03E224774), uint64_t(0x4FC8E9560F91B123), uint64_t(0x1DB956E450275779), uint64_t(0xB8D91274B9E9D4FB),
	uint64_t(0xA2EBEE47E2FBFCE1), uint64_t(0xD9F1F30CCD97FB09), uint64_t(0xEFED53D75FD64E6B), uint64_t(0x2E6D02C36017F67F),
	uint64_t(0xA9AA4D20DB084E9B), uint64_t(0xB64BE8D8B25396C1), uint64_t(0x70CB6AF7C2D5BCF0), uint64_t(0x98F076A4F7A2322E),
	uint64_t(0xBF84470805E69B5F), uint64_t(0x94C3251F06F90CF3), uint64_t(0x3E003E616A6591E9), uint64_t(0xB925A6CD0421AFF3),
	uint64_t(0x61BDD1307C66E300), uint64_t(0xBF8D5108E27E0D48), uint64_t(0x240AB57A8B888B20), uint64_t(0xFC87614BAF287E07),
	uint64_t(0xEF02CDD06FFDB432), uint64_t(0xA1082C0466DF6C0A), uint64_t(0x8215E577001332C8), uint64_t(0xD39BB9C3A48DB6CF),
	uint64_t(0x2738259634305C14), uint64_t(0x61CF4F94C97DF93D), uint64_t(0x1B6BACA2AE4E125B), uint64_t(0x758F450C88572E0B),
	uint64_t(0x959F587D507A8359), uint64_t(0xB063E962E045F54D), uint64_t(0x60E8ED72C0DFF5D1), uint64_t(0x7B64978555326F9F),
	uint64_t(0xFD080D236DA814BA), uint64_t(0x8C90FD9B083F4558), uint64_t(0x106F72FE81E2C590), uint64_t(0x7976033A39F7D952),
	uint64_t(0xA4EC0132764CA04B), uint64_t(0x733EA705FAE4FA77), uint64_t(0xB4D8F77BC3E56167), uint64_t(0x9E21F4F903B33FD9),
	uint64_t(0x9D765E419FB69F6D), uint64_t(0xD30C088BA61EA5EF), uint64_t(0x5D94337FBFAF7F5B), uint64_t(0x1A4E4822EB4D7A59),
	uint64_t(0x6FFE73E81B637FB3), uint64_t(0xDDF957BC36D8B9CA), uint64_t(0x64D0E29EEA8838B3), uint64_t(0x08DD9BDFD96B9F63),
	uint64_t(0x087E79E5A57D1D13), uint64_t(0xE328E230E3E2B3FB), uint64_t(0x1C2559E30F0946BE), uint64_t(0x720BF5F26F4D2EAA),
	uint64_t(0xB0774D261CC609DB), uint64_t(0x443F64EC5A371195), uint64_t(0x4112CF68649A260E), uint64_t(0xD813F2FAB7F5C5CA),
	uint64_t(0x660D3257380841EE), uint64_t(0x59AC2C7873F910A3), uint64_t(0xE846963877671A17), uint64_t(0x93B633ABFA3469F8),
	uint64_t(0xC0C0F5A60EF4CDCF), uint64_t(0xCAF21ECD4377B28C), uint64_t(0x57277707199B8175), uint64_t(0x506C11B9D90E8B1D),
	uint64_t(0xD83CC2687A19255F), uint64_t(0x4A29C6465A314CD1), uint64_t(0xED2DF21216235097), uint64_t(0xB5635C95FF7296E2),
	uint64_t(0x22AF003AB672E811), uint64_t(0x52E762596BF68235), uint64_t(0x9AEBA33AC6ECC6B0), uint64_t(0x944F6DE09134DFB6),
	uint64_t(0x6C47BEC883A7DE39), uint64_t(0x6AD047C430A12104), uint64_t(0xA5B1CFDBA0AB4067), uint64_t(0x7C45D833AFF07862),
	uint64_t(0x5092EF950A16DA0B), uint64_t(0x9338E69C052B8E7B), uint64_t(0x455A4B4CFE30E3F5), uint64_t(0x6B02E63195AD0CF8),
	uint64_t(0x6B17B224BAD6BF27), uint64_t(0xD1E0CCD25BB9C169), uint64_t(0xDE0C89A556B9AE70), uint64_t(0x50065E535A213CF6),
	uint64_t(0x9C1169FA2777B874), uint64_t(0x78EDEFD694AF1EED), uint64_t(0x6DC93D9526A50E68), uint64_t(0xEE97F453F06791ED),
	uint64_t(0x32AB0EDB696703D3), uint64_t(0x3A6853C7E70757A7), uint64_t(0x31865CED6120F37D), uint64_t(0x67FEF95D92607890),
	uint64_t(0x1F2B1D1F15F6DC9C), uint64_t(0xB69E38A8965C6B65), uint64_t(0xAA9119FF184CCCF4), uint64_t(0xF43C732873F24C13),
	uint64_t(0xFB4A3D794A9A80D2), uint64_t(0x3550C2321FD6109C), uint64_t(0x371F77E76BB8417E), uint64_t(0x6BFA9AAE5EC05779),
	uint64_t(0xCD04F3FF001A4778), uint64_t(0xE3273522064480CA), uint64_t(0x9F91508BFFCFC14A), uint64_t(0x049A7F41061A9E60),
	uint64_t(0xFCB6BE43A9F2FE9B), uint64_t(0x08DE8A1C7797DA9B), uint64_t(0x8F9887E6078735A1), uint64_t(0xB5B4071DBFC73A66),
	uint64_t(0x230E343DFBA08D33), uint64_t(0x43ED7F5A0FAE657D), uint64_t(0x3A88A0FBBCB05C63), uint64_t(0x21874B8B4D2DBC4F),
	uint64_t(0x1BDEA12E35F6A8C9), uint64_t(0x53C065C6C8E63528), uint64_t(0xE34A1D250E7A8D6B), uint64_t(0xD6B04D3B7651DD7E),
	uint64_t(0x5E90277E7CB39E2D), uint64_t(0x2C046F22062DC67D), uint64_t(0xB10BB459132D0A26), uint64_t(0x3FA9DDFB67E2F199),
	uint64_t(0x0E09B88E1914F7AF), uint64_t(0x10E8B35AF3EEAB37), uint64_t(0x9EEDECA8E272B933), uint64_t(0xD4C718BC4AE8AE5F),
	uint64_t(0x81536D601170FC20), uint64_t(0x91B534F885818A06), uint64_t(0xEC8177F83F900978), uint64_t(0x190E714FADA5156E),
	uint64_t(0xB592BF39B0364963), uint64_t(0x89C350C893AE7DC1), uint64_t(0xAC042E70F8B383F2), uint64_t(0xB49B52E587A1EE60),
	uint64_t(0xFB152FE3FF26DA89), uint64_t(0x3E666E6F69AE2C15), uint64_t(0x3B544EBE544C19F9), uint64_t(0xE805A1E290CF2456),
	uint64_t(0x24B33C9D7ED25117), uint64_t(0xE74733427B72F0C1), uint64_t(0x0A804D18B7097475), uint64_t(0x57E3306D881EDB4F),
	uint64_t(0x4AE7D6A36EB5DBCB), uint64_t(0x2D8D5432157064C8), uint64_t(0xD1E649DE1E7F268B), uint64_t(0x8A328A1CEDFE552C),
	uint64_t(0x07A3AEC79624C7DA), uint64_t(0x84547DDC3E203C94), uint64_t(0x990A98FD5071D263), uint64_t(0x1A4FF12616EEFC89),
	uint64_t(0xF6F7FD1431714200), uint64_t(0x30C05B1BA332F41C), uint64_t(0x8D2636B81555A786), uint64_t(0x46C9FEB55D120902),
	uint64_t(0xCCEC0A73B49C9921), uint64_t(0x4E9D2827355FC492), uint64_t(0x19EBB029435DCB0F), uint64_t(0x4659D2B743848A2C),
	uint64_t(0x963EF2C96B33BE31), uint64_t(0x74F85198B05A2E7D), uint64_t(0x5A0F544DD2B1FB18), uint64_t(0x03727073C2E134B1),
	uint64_t(0xC7F6AA2DE59AEA61), uint64_t(0x352787BAA0D7C22F), uint64_t(0x9853EAB63B5E0B35), uint64_t(0xABBDCDD7ED5C0860),
	uint64_t(0xCF05DAF5AC8D77B0), uint64_t(0x49CAD48CEBF4A71E), uint64_t(0x7A4C10EC2158C4A6), uint64_t(0xD9E92AA246BF719E),
	uint64_t(0x13AE978D09FE5557), uint64_t(0x730499AF921549FF), uint64_t(0x4E4B705B92903BA4), uint64_t(0xFF577222C14F0A3A),
	uint64_t(0x55B6344CF97AAFAE), uint64_t(0xB862225B055B6960), uint64_t(0xCAC09AFBDDD2CDB4), uint64_t(0xDAF8E9829FE96B5F),
	uint64_t(0xB5FDFC5D3132C498), uint64_t(0x310CB380DB6F7503), uint64_t(0xE87FBB46217A360E), uint64_t(0x2102AE466EBB1148),
	uint64_t(0xF8549E1A3AA5E00D), uint64_t(0x07A69AFDCC42261A), uint64_t(0xC4C118BFE78FEAAE), uint64_t(0xF9F4892ED96BD438),
	uint64_t(0x1AF3DBE25D8F45DA), uint64_t(0xF5B4B0B0D2DEEEB4), uint64_t(0x962ACEEFA82E1C84), uint64_t(0x046E3ECAAF453CE9),
	uint64_t(0xF05D129681949A4C), uint64_t(0x964781CE734B3C84), uint64_t(0x9C2ED44081CE5FBD), uint64_t(0x522E23F3925E319E),
	uint64_t(0x177E00F9FC32F791), uint64_t(0x2BC60A63A6F3B3F2), uint64_t(0x222BBFAE61725606), uint64_t(0x486289DDCC3D6780),
	uint64_t(0x7DC7785B8EFDFC80), uint64_t(0x8AF38731C02BA980), uint64_t(0x1FAB64EA29A2DDF7), uint64_t(0xE4D9429322CD065A),
	uint64_t(0x9DA058C67844F20C), uint64_t(0x24C0E332B70019B0), uint64_t(0x233003B5A6CFE6AD), uint64_t(0xD586BD01C5C217F6),
	uint64_t(0x5E5637885F29BC2B), uint64_t(0x7EBA726D8C94094B), uint64_t(0x0A56A5F0BFE39272), uint64_t(0xD79476A84EE20D06),
	uint64_t(0x9E4C1269BAA4BF37), uint64_t(0x17EFEE45B0DEE640), uint64_t(0x1D95B0A5FCF90BC6), uint64_t(0x93CBE0B699C2585D),
	uint64_t(0x65FA4F227A2B6D79), uint64_t(0xD5F9E858292504D5), uint64_t(0xC2B5A03F71471A6F), uint64_t(0x59300222B4561E00),
	uint64_t(0xCE2F8642CA0712DC), uint64_t(0x7CA9723FBB2E8988), uint64_t(0x2785338347F2BA08), uint64_t(0xC61BB3A141E50E8C),
	uint64_t(0x150F361DAB9DEC26), uint64_t(0x9F6A419D382595F4), uint64_t(0x64A53DC924FE7AC9), uint64_t(0x142DE49FFF7A7C3D),
	uint64_t(0x0C335248857FA9E7), uint64_t(0x0A9C32D5EAE45305), uint64_t(0xE6C42178C4BBB92E), uint64_t(0x71F1CE2490D20B07),
	uint64_t(0xF1BCC3D275AFE51A), uint64_t(0xE728E8C83C334074), uint64_t(0x96FBF83A12884624), uint64_t(0x81A1549FD6573DA5),
	uint64_t(0x5FA7867CAF35E149), uint64_t(0x56986E2EF3ED091B), uint64_t(0x917F1DD5F8886C61), uint64_t(0xD20D8C88C8FFE65F),
	uint64_t(0x31D71DCE64B2C310), uint64_t(0xF165B587DF898190), uint64_t(0xA57E6339DD2CF3A0), uint64_t(0x1EF6E6DBB1961EC9),
	uint64_t(0x70CC73D90BC26E24), uint64_t(0xE21A6B35DF0C3AD7), uint64_t(0x003A93D8B2806962), uint64_t(0x1C99DED33CB890A1),
	uint64_t(0xCF3145DE0ADD4289), uint64_t(0xD0E4427A5514FB72), uint64_t(0x77C621CC9FB3A483), uint64_t(0x67A34DAC4356550B),
	uint64_t(0xF8D626AAAF278509),
};




