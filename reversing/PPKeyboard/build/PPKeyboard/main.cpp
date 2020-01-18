#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

#include <Windows.h>
#include <mmsystem.h>


#pragma comment(lib, "winmm.lib")


void PrintInputKey(DWORD dwParam1) {
	if (dwParam1 > 0x007f0000) {
		if ((dwParam1 & 0xff) == 0x97) {
			printf("0x%x", ((dwParam1 & 0x0fff) - 0x97) / 0x0100);
		}
		else if ((dwParam1 & 0xff) == 0x99) {
			printf("%x ", ((dwParam1 & 0x0fff) - 0x99) / 0x0100);
		}
	}
}


void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2) {
	switch (wMsg) {
		case MIM_DATA:
			PrintInputKey(dwParam1);
			break;
	}

	return;
}


int main(int argc, char* argv[]) {
	HMIDIIN hMidiDevice = NULL;
	DWORD nMidiPort = -1;
	UINT nMidiDeviceNum;
	MIDIINCAPS caps;
	MMRESULT result;

	nMidiDeviceNum = midiInGetNumDevs();

	if (nMidiDeviceNum == 0) {
		return -1;
	}

	for (unsigned int i = 0; i < nMidiDeviceNum; i++) {
		midiInGetDevCaps(i, &caps, sizeof(MIDIINCAPS));

		if (strcmp(caps.szPname, "DDJ-XP1") == 0) {
			nMidiPort = i;

			break;
		}
	}

	if (nMidiPort == -1) {
		return -1;
	}

	result = midiInOpen(&hMidiDevice, nMidiPort, (DWORD_PTR)(void*)MidiInProc, 0, CALLBACK_FUNCTION);

	if (result != MMSYSERR_NOERROR) {
		return -1;
	}

	midiInStart(hMidiDevice);

	while (true) {
		if (!_kbhit()) {
			Sleep(100);

			continue;
		}

		int c = _getch();

		if (c == 'x') {
			system("cls");
		}

		if (c == VK_ESCAPE || c == 'q') {
			break;
		}
	}

	midiInStop(hMidiDevice);
	midiInClose(hMidiDevice);

	hMidiDevice = NULL;
	nMidiPort = -1;

	return 0;
}
