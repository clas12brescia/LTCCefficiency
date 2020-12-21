# Passwordless git

1. On your local machine generate a ssh private/public key:
   
   ```bash
   ssh-keygen -t rsa
   ```

2. Go to you Github [profile settings](https://github.com/settings/profile) > [SSH-GPG keys](https://github.com/settings/keys) and add the public key

3. On your local machine, in the repository directory
   
   ```bash
   git remote set-url origin git+ssh://git@github.com/clas12brescia/LTCCefficiency.git
   ```
