# Install required dependencies for CentOS systems
yum update -y
yum install -y "Development Tools"
yum install -y python3 which tree
python3 -m pip install --upgrade pip
python3 -m pip install pipenv
python3 -m pip install wheel

