#!groovy
pipeline {
    options {
        skipDefaultCheckout(true)
    }
    agent {
        label {
            label ""
            customWorkspace "/var/jenkins_home/workspace/importweahter/"
        } // Ende Label
    } // Ende Agent
    triggers {
        //Execute weekdays every four hours starting at minute 0
        cron('25 1 * * *')
        pollSCM('1 1 * * *')
    } // Ende Triggers
     stages {
        stage('Prepare') {
            steps {
                // GIT submodule recursive checkout
                checkout scm: [
                        $class: 'GitSCM',
                        branches: scm.branches,
                        doGenerateSubmoduleConfigurations: false,
                        extensions: [[$class: 'SubmoduleOption',
                                      disableSubmodules: false,
                                      parentCredentials: false,
                                      recursiveSubmodules: true,
                                      reference: '',
                                      trackingSubmodules: false]],
                        submoduleCfg: [],
                        userRemoteConfigs: scm.userRemoteConfigs
                ]
                sh 'git submodule update --init --recursive'
                sh 'git submodule update'
                // copy managed files to workspace
                sh 'mkdir -p bin'
                sh 'mkdir -p coverage'
            }
        }
		stage('build frameworks') {
		    steps {
                parallel (
                    "curlpp" : {
		            sh """
			    cd frameworks/curlpp
                            mkdir -p build
                            cd build
                            cmake ..
                            make
                            sudo make install
			    """
                    }

				) // Ende Parallel
            } // Ende Framework Steps
        } // Ende Stage build framework

        stage('build') {
            steps {
                parallel (
                    "ReadWeatherSource" : {
                        sh 'make'
                    }
		          )
		        }
		    } // Ende build


     } // Ende Stages
    post {
        always {
            deleteDir()
        }
    } // Ende Post


 } // Ende Pipeline
