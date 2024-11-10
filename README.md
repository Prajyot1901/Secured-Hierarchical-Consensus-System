# Secured-Hierarchical-Consensus-System

## Overview

The *Secured-Hierarchical-Consensus-System* is a robust solution for conducting secure and hierarchical voting among registrars, sub-registrars, and associate registrars. This system ensures confidentiality and integrity in validating land transactions by leveraging *Paillier's encryption system*. It enables a consensus mechanism where randomly chosen registrars assess the validity of land transactions and securely cast their votes.

## Key Features
- *Hierarchical Voting*: Conducts smooth and secure voting among registrars, sub-registrars, and associate registrars for transaction validation.
- *Paillier’s Encryption*: Encrypts and decrypts votes (yes/no) using public keys (n, g), ensuring the security of the process.
- *Digital Signatures*: Each vote includes a registrar’s digital signature to authenticate responses.
- *Homomorphic Operations*: Uses homomorphic properties of Paillier encryption to combine votes without decrypting individual responses.
- *Transaction Handling*: All transactions are stored in files with future integration into blockchain technology for added security and immutability.
- *Blockchain Integration*: Future development will involve creating blockchain blocks to securely store land transactions.
- *C++ Implementation*: The entire system is developed using C++ for encryption, decryption, and consensus operations.
- *GMP Library*: Utilizes the GMP library for big integer operations, essential for Paillier key generation and cryptographic operations.

## How It Works
1. *Transaction Submission*: A land transaction is submitted for validation and randomly assigned to selected registrars, sub-registrars, and associate registrars.
2. *Registrar Voting*: Each registrar assesses the validity of the transaction and responds with a yes/no vote. Their digital signature is embedded in the message.
3. *Encryption*: The response is encrypted using the Paillier public key (n, g) and sent back.
4. *Homomorphic Combination*: The system uses homomorphic operations to combine the encrypted responses into a single encrypted result.
5. *Decryption*: The final combined encrypted result is decrypted using Paillier’s decryption method, revealing the consensus.
6. *Result Announcement*: The overall result is announced to all participants.
7. *Blockchain Integration*: Transactions will be stored in a blockchain system, creating immutable records of validated land transactions.

## Technical Details
- *Encryption System*: Paillier cryptosystem
- *Programming Language*: C++
- *Key Generation*: GMP library for handling large integers
- *Homomorphic Operations*: Applied to combine encrypted votes

## Prerequisites
- *GMP Library*: Ensure the GMP library is installed for handling large integers in the encryption and decryption processes.
- *C++ Compiler*: A C++ compiler supporting the GMP library is required.

## Installation
1. Clone the repository:
   bash
   git clone https://github.com/Prajyot1901/Secured-Hierarchical-Consensus-System.git

### Install the GMP library:

#### For Linux (Ubuntu/Debian):
bash
sudo apt-get update
sudo apt-get install libgmp-dev


#### For macOS:
Install GMP via Homebrew:
bash
brew install gmp


#### For Windows:
1. Download the GMP binaries from the official site: https://gmplib.org
2. Extract the contents of the downloaded file.
3. Follow the installation instructions in the [GMP Installation Guide](https://gmplib.org/manual/Installing-GMP.html#Installing-GMP).
4. Set the path to the GMP library in your environment variables.

### Compile the project:
bash
g++ -o consensus_system main.cpp -lgmp


### Run the system:
bash
./consensus_system
```

## Summary
This project focuses on building a secure hierarchical voting system to validate land transactions using Paillier’s encryption system. Future enhancements include integrating blockchain technology to store transactions and developing a secure online land registration platform.
