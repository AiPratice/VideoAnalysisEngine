<template>
    <div>
        <Row v-for="(result,videoId) in results" :key="videoId">
            <Card>
                <p slot="title">
                    <span v-if="videosMap[videoId]">{{videosMap[videoId]['name']}}</span>
                </p>
                <Row :gutter=16>
                    <Col span="6" v-for="item in result" :key="item.frameId">
                        <img :src="frameImgUrl+item.frameId" width="100%" @click="play(videoId,item.second)"></img>
                    </Col>
                </Row>
            </Card>
        </Row>

        <!-- 视频播放对话框 -->
        <Modal
            v-model="playerModal"
            title="视频播放"
            @on-ok="playerModalOk"
            @on-cancel="playerModalCancel">
            <video id="videoPlayer" :src="videoUrl+playvideoID" controls="controls" v-if="playerModal" width="100%">
                无法播放视频
            </video>
        </Modal>
    </div>
</template>
<script>
import util from '../../libs/util';
import config from '../../config';

export default {
    name: 'tag_search_results',
    mounted () {
        let objects = [],people = [];
        if (this.$route.query.objects) {
            objects = JSON.parse(this.$route.query.objects);
        }
        if (this.$route.query.people) {
            people = JSON.parse(this.$route.query.people);
        }
        util.ajax.post('/videos/tag-search', {
            objects:objects,
            people:people
        }).then((response) => {
            console.log(response.data);
            this.results = {};
            for(let i in response.data) {
                let videoId = response.data[i]['video_id'];
                if(!this.results[videoId]) {
                    this.results[videoId] = [];    
                }
                this.results[videoId].push({
                    frameId:response.data[i]['frame_id'],
                    second:response.data[i]['second']
                });
            }
            console.log(this.results);
        }).catch((response)=>{
            alert('标签搜素请求失败');
        });

        this.$store.dispatch('getVideos');
    },
    computed: {
        videosMap() {
            return this.$store.state.video.videosMap;
        }
    },
    data () {
        return {
            results:{},
            frameImgUrl:config.HOST+'/frames/frame_img?frame_id=',
            videoUrl:config.HOST+'/videos/video?video_id=',
            playerModal:false, //播放对话框
            playVideoID:0, //被播放的视频id
            playSecond:0, //播放视频的定位秒数
            videoID:0
        }
    },
    methods: {
        play(videoID,second){
            console.log(videoID);
            console.log(second);
            //视频播放
            this.playvideoID = videoID;
            this.playSecond = second;
            this.playerModal = true;

            setTimeout(()=>{
                let searchVideoPlayer = document.getElementById('videoPlayer');
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
}
</script>
