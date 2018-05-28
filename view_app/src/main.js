import Vue from 'vue';
import iView from 'iview';
import { router } from './router/index';
import { appRouter } from './router/router';
import store from './store';
import App from './app.vue';
import '@/locale';
import 'iview/dist/styles/iview.css';
import VueI18n from 'vue-i18n';
import VueNativeSock from 'vue-native-websocket';
import { VirtualScroller } from 'vue-virtual-scroller';
import Config from './config';
import wsHandler from './websocket/handler'

Vue.use(VueI18n);
Vue.use(iView);
Vue.use(VueNativeSock, Config.WS_HOST, {
    reconnection: true,
    reconnectionAttempts: 5,
    reconnectionDelay: 3000,
    format: 'json'
});
Vue.component('virtual-scroller', VirtualScroller);

new Vue({
    el: '#app',
    router: router,
    store: store,
    render: h => h(App),
    data: {
        currentPageName: ''
    },
    mounted() {
        this.currentPageName = this.$route.name;
        // 显示打开的页面的列表
        this.$store.commit('setOpenedList');
        this.$store.commit('initCachepage');
        // 权限菜单过滤相关
        this.$store.commit('updateMenulist');
    },
    created() {
        let tagsList = [];
        appRouter.map((item) => {
            if (item.children.length <= 1) {
                tagsList.push(item.children[0]);
            } else {
                tagsList.push(...item.children);
            }
        });
        this.$store.commit('setTagsList', tagsList);

        //响应websocket服务端的消息
        this.$options.sockets.onmessage = (response)=>{
            wsHandler(this,response);
        };
    }
});
