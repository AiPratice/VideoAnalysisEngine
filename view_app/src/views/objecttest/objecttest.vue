<style lang="less">
@import url("./objecttest.less");
</style>
<template>
    <div class="objecttest_main">
        <Row :gutter="16">
            <Col span="12">
                <Card>
                    <p slot="title">
                        <Icon type="search"></Icon>
                        物体检测测试
                    </p>
                    <Upload 
                        name="file"
                        type="drag" 
                        v-bind:action="uploadActionUrl" 
                        :format="['jpg','jpeg']"
                        :max-size="5120"
                        :before-upload="beforeUpload"
                        :on-exceeded-size="handleMaxSize"
                        :on-success="uploadOnSuccess"
                        :on-progress="handleUploadProgress"
                        :show-upload-list="false"
                        v-if="!isProcessing"
                        >
                        <div class="upload_container_box" style="border:none">
                            <Icon type="ios-cloud-upload" size="65" style="color: #3399ff"></Icon>
                            <p>单击或拖动图像进行文件上传</p>
                        </div>
                    </Upload>
                    <div class="upload_container_box" v-else>
                        <Icon type="loop" size="65" style="color: #3399ff"></Icon>
                        <p>正在对图像进行处理，请稍后...</p>
                    </div>
                </Card>
                
            </Col>
            <Col span="12">
                
            </Col>
        </Row>
        <br/>
        <Row :gutter="16">
            <Col span="24">
                <Card>
                    <p slot="title">
                        <Icon type="image"></Icon>
                        识别结果
                    </p>
                    <div class="result_container_box">
                        <img :src="previewImgSrc" v-if="previewImgSrc.length>0"/>

                        <div class="detect_result_item" 
                            v-for="(result,index) in results"
                            :key="index" 
                            v-bind:style="{top:result.top+'px', left:result.left+'px', width:result.width+'px', height:result.height+'px'}">
                            <div class="object_label">{{result.class_name}}</div>    
                        </div>
                    </div>
                </Card>
            </Col>
            
        </Row>

        <!-- 视频播放对话框 -->
        <Modal
            v-model="playerModal"
            title="视频播放"
            @on-ok="playerModalOk"
            @on-cancel="playerModalCancel">
            <video id="searchVideoPlayer" :src="videoUrl+playvideoID" controls="controls" v-if="playerModal" width="100%">
                无法播放视频
            </video>
        </Modal>
    </div>
</template>
<script>
import config from "../../config"
import util from "../../libs/util"

export default {
    name:'objecttest_index',
    mounted(){
        
    },
    data() {
        return {
            videoUrl:config.HOST+'/videos/video?video_id=',
            uploadActionUrl: config.HOST+"/objects/detect",
            results:[],//搜索结果图像
            previewImgSrc:'',
            isProcessing:false,
            playerModal:false, //播放对话框
            playVideoID:0, //被播放的视频id
            playSecond:0 //播放视频的定位秒数
        }
    },
    methods:{
        beforeUpload(file){
            if(file){
                this.isProcessing = true;
                let reader = new FileReader();
                reader.readAsDataURL(file);
                reader.addEventListener('load',()=>{
                    this.previewImgSrc = reader.result;
                });
            }
        },
        uploadOnSuccess(resp,file){
            this.results = resp;
            this.isProcessing = false;
        },
        handleMaxSize(){

        },
        handleUploadProgress(event, file, fileList){
            console.log(event);
            console.log(file);
            console.log(fileList);
        },
        playVideo(videoID,second){
            console.log(videoID);
            console.log(second);
            //视频播放
            this.playvideoID = videoID;
            this.playSecond = second;
            this.playerModal = true;

            setTimeout(()=>{
                let searchVideoPlayer = document.getElementById('searchVideoPlayer');
                searchVideoPlayer.currentTime = second;
            },300);
        },
        playerModalOk(){
            this.playerModal = false;
        },
        playerModalCancel(){
            this.playerModal = false;
        }
    }
};
</script>

