###Hierarchical Meta-Storms installer
###Bioinformatics Group, Single-Cell Research Center, QIBEBT, CAS
###Updated at Jan 30, 2019 
###Updated by Xiaoquan Su, Honglei Wang
#!/bin/bash
##Users can change the default environment variables configuration file here
if [[ $SHELL = '/bin/zsh' ]];
then
        PATH_File=~/.zshrc
        if [ ! -f "$PATH_File" ]
        then
                PATH_File=~/.zsh_profile
                if [ ! -f "$PATH_File" ]
                then
                        touch $PATH_File
                fi
        fi
else
        PATH_File=~/.bashrc
        if [ ! -f "$PATH_File" ]
        then
                PATH_File=~/.bash_profile
                if [ ! -f "$PATH_File" ]
                then
                        touch $PATH_File
                fi
        fi

fi
PM_PATH=`pwd`
Sys_ver=`uname`
###Checking that environment variable of Parallel-META exists###
Check_old_pm=`grep "export HierarchicalMetaStorms"  $PATH_File|awk -F '=' '{print $1}'`
Check_old_path=`grep "HierarchicalMetaStorms/bin"  $PATH_File |sed 's/\(.\).*/\1/' |awk '{if($1!="#"){print "Ture";}}'`
Add_Part="####DisabledbyHierarchicalMetaStorms####"
echo "**Hierarchical Meta-Storms Installation**"
echo "**version 1.0**"
###Build source code for src package###
if [ -f "Makefile" ]
   then
       echo -e "\n**Hierarchical Meta-Storms src package**"
       make
       echo -e "\n**Build Complete**"
else
   echo -e "\n**Hierarchical Meta-Storms bin package**"
fi
###Configure environment variables###

if [ "$Check_old_pm" != "" ]
   then
      Checking=`grep ^export\ HierarchicalMetaStorms  $PATH_File|awk -F '=' '{print $2}'`
      if [ "$Checking" != "$PM_PATH" ]
         then
         if [ "$Sys_ver" = "Darwin" ]
            then
            sed -i "" "s/^export\ HierarchicalMetaStorms/$Add_Part\ &/g" $PATH_File
            sed -i "" -e $"`grep -n "$Add_Part" $PATH_File | cut -d ":" -f 1 | head -1` a\ 
            export\ HierarchicalMetaStorms=$PM_PATH
            " $PATH_File
         else
             sed -i "s/^export\ HierarchicalMetaStorms/$Add_Part\ &/g" $PATH_File
             sed -i "/$Add_Part\ export\ HierarchicalMetaStorms/a export\ HierarchicalMetaStorms=$PM_PATH" $PATH_File
         fi
     fi    
elif [ "$Check_old_pm" = "" ]
    then
      echo "export HierarchicalMetaStorms="$PM_PATH >> $PATH_File
fi
if [ "$Check_old_path" = "" ]
    then
      echo "export PATH=\$PATH:\$HierarchicalMetaStorms/bin" >> $PATH_File
fi
###Source the environment variable file###
source $PATH_File
echo -e "\n**Environment Variables Configuration Complete**"
echo -e "\n**Hierarchical Meta-Storms Installation Complete**"
echo -e "\n**An example dataset with demo script is available in \"example\"**"
