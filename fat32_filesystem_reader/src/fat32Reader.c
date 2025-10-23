#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <math.h>

#include "A3FAT32.h"
#include "A3FsInfo.h"
#include "A3Dir.h"

// define variables
#define UL unsigned long
#define UC unsigned char
#define BUFFER_SIZE 512

int fileFD;
char *buffer;
UL dataSecStart;
UL fatStart;
fat32BS bootSector;
DIR dirStruct;
FSI fsiStruct;

// functions
void listDir(UL currentCluster, int depth);
void appendDot(char file[]);
void removeSpaces(char source[], char dest[]);
UL findFirstSector(UL clusterNumber);
void readFAT(UL sectorNumber, unsigned char *buffer, UL offset);
void print_filename(const uint8_t *filename);
UL getClusterNum(DIR dirEntry);
void readEntry(char *entryBuffer, UL sectorNumber, UL entryNumber);
void readCurrentFAT(UC *fatBuffer, UL sectorNumber, UL fatOffset);
bool findFile(UL currentCluster, char *input, DIR *currentEntry);
void fetchFile(UL clusterNum);

// main
int main(int argc, char **argv)
{
    printf("\n--------------------Program Start-----------------------\n");
    // receive program - check argc
    if (argc <= 2)
    {
        // error statement or print error message
        printf("\n Error: No Commands \n");
        printf("\n--------------------Program End-----------------------\n");
        exit(0);
    }
    if (argc > 2)
    {

        int count;
        buffer = malloc(BUFFER_SIZE);

        //=========Process 1: Open image=========
        fileFD = open(argv[1], O_RDONLY);
        if (!fileFD)
        {
            printf("\n Error: file not available \n");
            exit(EXIT_FAILURE);
        }
        //==========Finsih Process 1=============

        //=========Porcess 2: Read Sectors=========
        count = read(fileFD, buffer, 512);
        printf("Read count from file: %d\n", count);
        if (!count)
        {
            printf("Error (%d) - Boot Sector \n", count);
            exit(0);
        }
        memcpy(&bootSector, buffer, sizeof(fat32BS));

        //==========Finsih Process 2=============

        //========Porcess 3: Data Calculation/Retrieve=========

        UL bps = bootSector.BPB_BytesPerSec;
        UL spc = bootSector.BPB_SecPerClus;

        lseek(fileFD, bootSector.BPB_FSInfo * bps, SEEK_SET);
        count = read(fileFD, buffer, 1 * bps);
        if (!count)
        {
            printf("Error (%d) - FSI Sector \n", count);
            exit(0);
        }
        memcpy(&fsiStruct, buffer, sizeof(FSI));

        if (fsiStruct.lead_sig != 0x41615252 || fsiStruct.trail_signature != 0xAA550000)
        {
            printf("Signatures don't match: 0x41615252 vs 0x%x|| 0xAA550000 vs 0x%x\n", fsiStruct.lead_sig, fsiStruct.trail_signature);
        }
        UL freeSpace = fsiStruct.free_count * spc * bps;

        UL totalFATSize = bootSector.BPB_NumFATs * bootSector.BPB_FATSz32;
        UL remainFATSector = totalFATSize - bootSector.BPB_RsvdSecCnt;
        UL remainFATByte = remainFATSector * spc * bps;
        // UL totalFATByte = (totalFATSize * bps);ß
        UL DataSec = bootSector.BPB_TotSec32 - (bootSector.BPB_RsvdSecCnt + (bootSector.BPB_NumFATs * bootSector.BPB_FATSz32) + 0);
        UL countOfClusters = DataSec / spc;

        char volumeLabel[12];
        memcpy(volumeLabel, bootSector.BS_VolLab, 11);
        volumeLabel[11] = '\0';

        //===========Finsih Process 3==================

        if (argc == 3 && strcmp(argv[2], "info") == 0)
        {

            printf("\n----Command:info -> printing information of the file-----\n");
            // Drive name
            printf("\nOEMName: %s\n", bootSector.BS_OEMName);
            printf("\nVolumen Label: %s\n", volumeLabel);//bootSector.BS_VolLab -- have error
            // Free Space
            printf("\nBytes per Sector: %ld\n", bps);
            printf("\nSector per Cluster: %ld\n", spc);
            printf("\nFree space on the drive: %ld (kB)\n", freeSpace / 1024);
            // Amount on usable storage
            printf("\nThe amount of Usable Storage: %ld (kB)\n", remainFATByte / 1024);
            // cluster size in number of sector
            printf("\nThe count of clusters: %ld \n", countOfClusters);
            printf("\nThe count of clusters in Sector: %ld (Sector)\n", countOfClusters);
            printf("\nThe count of clusters in Kb: %ld (kB)\n", countOfClusters * bps / 1024);

            // printf("\nThe cluster size in Sector: %ld (Sector)\n", totalFATSize);
            // printf("\nThe cluster size in Kb: %ld (kB)\n", totalFATByte / 1024);
        }
        else if (argc == 3 && strcmp(argv[2], "list") == 0)
        {
            // Output all files and directories on the drive.
            printf("\n----Command:list -> printing directory/file in drive-----\n");
            // find data start address
            fatStart = bootSector.BPB_RsvdSecCnt;
            dataSecStart = bootSector.BPB_RsvdSecCnt + bootSector.BPB_NumFATs * bootSector.BPB_FATSz32;

            // list Directory
            listDir(bootSector.BPB_RootClus, 0);
        }
        else if (argc == 4 && strcmp(argv[2], "get") == 0)
        {
            printf("\n----Command:get -> fetching information of the file-----\n");
            // Be able to fetch, and return a file from the drive
            fatStart = bootSector.BPB_RsvdSecCnt;
            dataSecStart = bootSector.BPB_RsvdSecCnt + bootSector.BPB_NumFATs * bootSector.BPB_FATSz32;
            UL currentCluster = bootSector.BPB_RootClus;
            // receive input and check
            char *input;
            bool found = false;
            bool eof = false;
            DIR *currentEntry = (DIR *)malloc(sizeof(DIR));
            input = strtok(argv[3], "/");
            // printf("Testing : input 1 %s\n", input);
            while (input != NULL && !eof)
            {

                //printf("Testing : input2 %s\n", input);
                found = findFile(currentCluster, input, currentEntry);

                input = strtok(NULL, "/");

                if (found)
                {
                    // get next directory cluster
                    //printf("Testing : found and currentCluster %lu\n", currentCluster);
                    currentCluster = currentEntry->dir_first_cluster_lo;
                    //printf("Testing : found and currentCluster %lu\n", currentCluster);
                    // if it is a file
                    if ((currentEntry->dir_attr & 0x20) != 0 && input == NULL)
                    {
                        //printf("Testing : it is end file and currentCluster %lu\n", currentCluster);
                        fetchFile(currentCluster);
                        eof = true;
                    }
                    else if ((currentEntry->dir_attr & 0x20) != 0 && input == NULL)
                    {
                        printf("Invalid file search syntax. \n");
                    }

                    // if it is a directory, continue reading
                    memset(currentEntry, 0, sizeof(DIR));
                }
            }
        }
        else
        {
            printf("\nInstruction uncleared.\n");
            printf("\n--------------------Program End-------------------------\n");
            exit(0);
        }
    }

    printf("\n--------------------Program End-------------------------\n");
    return 0;
}

// implementation of functions

//=========fetch the file to output.txt=========

void fetchFile(UL clusterNum)
{
    // Open the output file in write mode
    int outputFile = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // Error management
    if (outputFile == -1)
    {
        printf("Error: Unable to open output file.\n");
        return;
    }

    // Calculate the starting sector of the file data
    UL startSector = findFirstSector(clusterNum);

    // Set the file pointer to the starting sector
    off_t offset = startSector * bootSector.BPB_BytesPerSec;
    if (lseek(fileFD, offset, SEEK_SET) == -1)
    {
        printf("Error: Unable to seek to starting sector.\n");
        close(outputFile);
        return;
    }

    // =========Read data from the file and write it to the output file=========
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(fileFD, buffer, BUFFER_SIZE)) > 0)
    {
        if (write(outputFile, buffer, bytesRead) != bytesRead)
        {
            printf("Error: Unable to write to output file.\n");
            close(outputFile);
            return;
        }
    }

    // Check for read error
    if (bytesRead == -1)
    {
        printf("Error: Unable to read from input file.\n");
        close(outputFile);
        return;
    }

    // Close the output file
    if (close(outputFile) == -1)
    {
        printf("Error: Unable to close output file.\n");
        return;
    }

    printf("File successfully fetched and saved to output.txt.\n");
}

// ========Given the directory cluster, find the file/directory=========

bool findFile(UL currentCluster, char *input, DIR *currentEntry)
{

    UL spc = bootSector.BPB_SecPerClus;
    UL bps = bootSector.BPB_BytesPerSec;
    UL eps = spc * bps / 32; // directory entry per sector
    char name[12];
    char convertedName[12];
    UL fatOffset = (currentCluster) * 4;
    bool found = false;

    // find base sector
    UL baseSector = findFirstSector(currentCluster);

    for (int i = 0; i <(int) spc && !found; i++)
    {

        for (int j = 0; j <(int) eps && !found; j++)
        {

            readEntry((char *)currentEntry, baseSector + i, j);

            memcpy(name, currentEntry->dir_name, 11);
            name[11] = '\0';
            removeSpaces(name, convertedName);

            // convert name if it is file
            if ((currentEntry->dir_attr & 0x20) != 0)
            {

                appendDot(convertedName);
            }

            // if the file exists
            if (((currentEntry->dir_attr & 0x10) != 0 || (currentEntry->dir_attr & 0x20) != 0) && j > 2)
            {
                if (strcasecmp(convertedName, input) == 0)
                {
                    return found = true;
                }
            }
        }
    }

    // find next Cluster in FAT
    UC *fatBuffer = malloc(BUFFER_SIZE);
    readCurrentFAT(fatBuffer, fatStart, fatOffset);

    // Assuming each FAT entry is a 32-bit value
    UL *fatEntry = (UL *)fatBuffer;

    // Assuming fatEntryOffset is the offset within the FAT buffer
    UL nextCluster = fatEntry[0];
    nextCluster = nextCluster & 0x0FFFFFFF;

    if (nextCluster < 0xffffff7)
    {
        findFile(nextCluster, input, currentEntry);
    }

    return found;
}

// =========Remove spaces from given string=========

void removeSpaces(char source[], char dest[])
{
    int j = 0;

    for (int i = 0; i <(int) strlen(source); i++)
    {
        if (source[i] != ' ')
        {
            dest[j] = source[i];
            j++;
        }
    }
    dest[j] = '\0';
}

//========= append the dot between the file and format=========

void appendDot(char file[])
{
    char temp = '.';
    char tempChar;
    for (int i = strlen(file) - 3; i <(int) strlen(file) + 1; i++)
    {
        tempChar = file[i];
        file[i] = temp;
        temp = tempChar;
    }
}

// Returns cluster number from direct entry
// UL getClusterNum(DIR dirEntry)
// {
//     return ((dirEntry.dir_first_cluster_hi << 16) + dirEntry.dir_first_cluster_lo);
// }

//=========List all directory entry given the cluster=========

void listDir(UL currentCluster, int depth)
{

    UL spc = bootSector.BPB_SecPerClus;
    UL bps = bootSector.BPB_BytesPerSec;
    UL eps = spc * bps / 32; // directory entry per sector
    DIR *currEntry;
    char name[12];
    char convertedName[12];
    UL fatOffset = (currentCluster) * 4;
    char *entryBuffer = malloc(BUFFER_SIZE);

    // find base sector
    UL baseSector = findFirstSector(currentCluster);

    // read entry using for loop
    for (int i = 0; i <(int) spc; i++)
    {

        for (int j = 0; j <(int) eps; j++)
        {

            readEntry(entryBuffer, baseSector + i, j);

            currEntry = (DIR *)entryBuffer;
            memcpy(name, currEntry->dir_name, 11);
            name[11] = '\0';

            removeSpaces(name, convertedName);

            // base on entry attribute, determine print file/ search next directory
            if (((currEntry->dir_attr & 0x10) != 0 || (currEntry->dir_attr & 0x20) != 0) && j > 2)
            {

                printf("|");
                for (int k = 0; k < depth; k++)
                {
                    printf("_");
                }

                if ((currEntry->dir_attr & 0x10) != 0)

                {
                    //-> if it is a directory, retrieve the low index and call method again
                    if (currEntry->dir_attr != 0x0f)
                    {
                        printf("Directory: %s\n", convertedName);
                    }
                    listDir(currEntry->dir_first_cluster_lo, depth + 1);
                }
                else if ((currEntry->dir_attr & 0x20) != 0)
                {
                    // -> if it is a file. just print a name.
                    appendDot(convertedName);
                    printf("File: %s\n", convertedName);
                }
            }
        }
    }

    // find next Cluster in FAT
    UC *fatBuffer = malloc(BUFFER_SIZE);
    readCurrentFAT(fatBuffer, fatStart, fatOffset);

    // Assuming each FAT entry is a 32-bit value
    UL *fatEntry = (UL *)fatBuffer;

    // Assuming fatEntryOffset is the offset within the FAT buffer
    UL nextCluster = fatEntry[0];
    nextCluster = nextCluster & 0x0FFFFFFF;
    if (nextCluster < 0xffffff7)
    {

        listDir(nextCluster, depth);
    }

    //free(entryBuffer);
   // free(fatBuffer);
}

// UL findNextCluster(char* buffer, off_t fatOffset, UL currentCluster)
// {
//     printf("\naccess findNextCluster\n");
//     UL nextClusterNumber = 0;
//     lseek(fileFD, fatOffset + currentCluster * sizeof nextClusterNumber, SEEK_SET);
//     read(fileFD, buffer,BUFFER_SIZE);
//      printf("\nexit findNextClusterT\n");
//     return nextClusterNumber;
// }

//===============Get Information Into FatBuffer=============================

void readCurrentFAT(UC *fatBuffer, UL sectorNumber, UL fatOffset)
{

    UL bps = bootSector.BPB_BytesPerSec;
    lseek(fileFD, fatOffset + sectorNumber * bps, SEEK_SET);
    read(fileFD, fatBuffer, BUFFER_SIZE);
}

//===============Get Entry Information Into EntryBuffer===========================

void readEntry(char *entryBuffer, UL sectorNumber, UL entryNumber)
{
    UL bps = bootSector.BPB_BytesPerSec;
    UL entrySize = 32; // Size of each directory entry

    // Calculate the offset within the sector based on entry number and size
    off_t offset = sectorNumber * bps + entryNumber * entrySize;

    // Move the file pointer to the calculated offset
    lseek(fileFD, offset, SEEK_SET);

    // Read the directory entry into the entryBuffer
    read(fileFD, entryBuffer, entrySize);
}

// void readEntry2(char *entryBuffer, UL clusterNumber, UL entryNumber)
// {
//     UL spc = bootSector.BPB_SecPerClus;
//     UL bps = bootSector.BPB_BytesPerSec;
//     lseek(fileFD, dataSecStart * bps + (clusterNumber - 2) * spc * bps + entryNumber * 32, SEEK_SET);
//     read(fileFD, entryBuffer, BUFFER_SIZE);

//     // read the entry into EntryBuffer
// }

//=====Returns the first sector number for cluster=====

UL findFirstSector(UL clusterNumber)
{
    //  DataStartSector = BPB_ResvdSecCnt + BPB_FATSz * BPB_NumFATs;
    //  FirstSectorofCluster = DataStartSector + (N - 2) * BPB_SecPerClus;
    return dataSecStart + (clusterNumber - 2) * bootSector.BPB_SecPerClus;
}
