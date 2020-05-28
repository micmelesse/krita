SCRIPT_PATH=$(realpath $0)
SCRIPT_DIR_PATH=$(dirname $SCRIPT_PATH)
SRC_PATH=$(dirname $SCRIPT_DIR_PATH)
echo $SRC_PATH

cd /tmp
rm -rf krita-auto-1

# create directory structure for container control directory
git clone git://anongit.kde.org/scratch/dkazakov/krita-docker-env.git krita-auto-1

cd krita-auto-1
mkdir persistent

# copy/checkout Krita sources to 'persistent/krita'
cp -r $SRC_PATH ./persistent/krita

## or ...
# git clone kde:krita persistent/krita

# download the deps archive
./bin/bootstrap-deps.sh
./bin/build_image krita-deps
./bin/run_container krita-deps krita-auto-1