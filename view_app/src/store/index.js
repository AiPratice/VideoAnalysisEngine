import Vue from 'vue';
import Vuex from 'vuex';

import app from './modules/app';
import user from './modules/user';
import video from './modules/video';
import person from './modules/person';
import object from './modules/object';

Vue.use(Vuex);

const store = new Vuex.Store({
    state: {
        isLongTimeWorking:false
    },
    mutations: {
        setIsLongTimeWorking(state,isLongTimeWorking){
            state.isLongTimeWorking = isLongTimeWorking;
        }
    },
    actions: {

    },
    modules: {
        app,
        user,
        video,
        person,
        object
    }
});

export default store;
